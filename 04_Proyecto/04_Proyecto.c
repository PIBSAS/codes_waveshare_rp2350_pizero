#include <stdio.h>           // printf()
#include "pico/stdlib.h"     // Se usa stdio_init_all() y sleep_ms()
#include "pico/multicore.h"  // Se usa multicore_launch_core1()
#include "hardware/clocks.h" // Se usa set_sys_clock_khz()
#include "hardware/vreg.h"   // Se usa vreg_set_voltage()
#include "hardware/pio.h"    // Permite usar el bloqie PIO, DVI usa PIO para generar la senal TMDS.
#include "dvi.h"             // Control general DVI
#include "dvi_serialiser.h"  // Serializador TMDS
#include "dvi_timing.h"      // Configuracion de timings

#define FRAME_WIDTH 320      // Ancho del frambuffer
#define FRAME_HEIGHT 240     // Alto del frrambuffer
#define VREG_VSEL VREG_VOLTAGE_1_20         // Define el voltaje en 1.20 V
#define DVI_TIMING dvi_timing_640x480p_60hz // Selecciona la resolucion y refresco del video

/* Variable global tipo struct dvi_inst Instancia DVI Se almacena en memoria estatica */
struct dvi_inst dvi0;

/* Configuracion del serializador para la Waveshare RP2350-PiZero 
 * static: Visible solo en este archivo
 * const : NO se puede modificar en ejecucion
 * .pio Usa el bloque PIO0 del RP2350
 * .sm_tmds Usa 3 state machines del PIO
 * .pins_tmds GPIO usados para datos TMDS (D0, D1, D2)
 * .pins_clk GPIO usado para el clock HDMI
 * .invert_diffpairs No invierte pares diferenciales
 */
static const struct dvi_serialiser_cfg waveshare_hdmi_cfg = {
    .pio = pio0,
    .sm_tmds = {0, 1, 2},
    .pins_tmds = {36, 34, 32},
    .pins_clk =38,
    .invert_diffpairs = false
};

/* Cada pixel ocupa 16 bits, formato RGB565 */
uint16_t framebuffer[FRAME_WIDTH * FRAME_HEIGHT];

/* Funcion que ejecutara el nucleo 1 */
void core1_main()
{
    /* Registra las interrupciones DMA para este nucleo */
    dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);

    /* Espera hasta que haya datos validos.
     * queue_is_empty() => Verifica si la cola esta vacia
     * __wfe() => Wait For Event (instruccion ARM)
     */
    while (queue_is_empty(&dvi0.q_colour_valid))
    {
        __wfe();
    }

    /* Inicia transmision DVI */
    dvi_start(&dvi0);
    dvi_scanbuf_main_16bpp(&dvi0);
}

int main()
{
    /* Inicializa USB/serial */
    stdio_init_all();

    /* Subir voltaje core (importante para 640x480) y espera estabilizacion */
    vreg_set_voltage(VREG_VSEL);
    sleep_ms(10);

    /* Configurar clock para DVI */
    set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);

    /* Configurar base GPIO */
    pio_set_gpio_base(pio0, 16);

    /* Inicializar DVI */
    dvi0.timing = &DVI_TIMING; // Asigna el modo de video
    dvi0.ser_cfg = waveshare_hdmi_cfg; // Asigna configuracion de pines
    /* ASigna dos spinlocks para sincronizacion */
    dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());

    /* Lanza el nucleo 1 */
    multicore_launch_core1(core1_main);

    // Pintar pantalla roja
    for (int i = 0; i < FRAME_WIDTH * FRAME_HEIGHT; i++)
    {
        framebuffer[i] = 0xF800; // Rojo RGB565 0xF800
    }

    /* Bucle principal (infinito) */
    while (true) {
        /* Recorre cada linea horizontal */
        for (int y = 0; y < FRAME_HEIGHT; y++)
        {
            /* Puntero a la fila actual */
            uint16_t *scanline = &framebuffer[y * FRAME_WIDTH];

            /* Agrega la linea a la cola para que DVI la transmita */
            queue_add_blocking_u32(&dvi0.q_colour_valid, &scanline);

            /* Espera hasta que esa linea haya sido liberada.
             * Es sincronizacion entre CPU y DMA */
            while (queue_try_remove_u32(&dvi0.q_colour_free, &scanline))
            {} // busy wait, en realidad se escribe en una linea cerrado por ; sin llaves : while();
        }
    }
}
