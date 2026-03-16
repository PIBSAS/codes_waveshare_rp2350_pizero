/**
 * 08_Proyecto_PICO-PIO-USB.c - Versión con detección real
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "pio_usb.h"

#define PIN_D_PLUS  28
#define PIN_D_MINUS 29

static usb_device_t *usb_device = NULL;
static bool teclado_conectado = false;
static bool hub_detectado = false;
static uint32_t frame_count = 0;

// Función mejorada para interpretar líneas USB
void interpretar_lineas_usb() {
    bool dp = gpio_get(PIN_D_PLUS);
    bool dm = gpio_get(PIN_D_MINUS);
    
    printf("Líneas USB: D+ = %d, D- = %d ", dp, dm);
    
    if (!dp && !dm) {
        printf("(SE0 - Bus reset o desconectado)\n");
    } else if (dp && !dm) {
        printf("(J State - Full Speed idle o dispositivo FS conectado)\n");
    } else if (!dp && dm) {
        printf("(K State - Low Speed idle o dispositivo LS conectado)\n");
    } else if (dp && dm) {
        printf("(SE1 - Error en el bus)\n");
    }
}

void print_hid_report(uint8_t dev_addr, uint8_t const *report, uint16_t len) {
    printf("\n[Teclado %d] ", dev_addr);
    
    if (len >= 8) {
        // Modificadores (Ctrl, Shift, Alt, Win)
        if (report[0] & 0x01) printf("Ctrl-Izq ");
        if (report[0] & 0x02) printf("Shift-Izq ");
        if (report[0] & 0x04) printf("Alt-Izq ");
        if (report[0] & 0x08) printf("Win-Izq ");
        if (report[0] & 0x10) printf("Ctrl-Der ");
        if (report[0] & 0x20) printf("Shift-Der ");
        if (report[0] & 0x40) printf("Alt-Der ");
        if (report[0] & 0x80) printf("Win-Der ");
        
        // Teclas
        printf("| Teclas: ");
        for (int i = 2; i < 8; i++) {
            if (report[i] != 0) {
                printf("0x%02X ", report[i]);
            }
        }
    }
    printf("\n");
}

int main() {
    stdio_init_all();
    sleep_ms(3000);
    
    printf("\n========================================\n");
    printf("PIO-USB HOST - Detección Real\n");
    printf("========================================\n");
    
    if (!set_sys_clock_khz(120000, true)) {
        printf("ERROR: No se pudo configurar el reloj\n");
        return -1;
    }
    printf("✓ Reloj configurado a 120MHz\n");
    
    // IMPORTANTE: Configurar pines con pull-down
    gpio_init(PIN_D_PLUS);
    gpio_init(PIN_D_MINUS);
    gpio_set_dir(PIN_D_PLUS, GPIO_IN);
    gpio_set_dir(PIN_D_MINUS, GPIO_IN);
    gpio_pull_down(PIN_D_PLUS);
    gpio_pull_down(PIN_D_MINUS);
    printf("✓ Pines GPIO%d y GPIO%d configurados con pull-down\n", 
           PIN_D_PLUS, PIN_D_MINUS);
    
    printf("\n--- Estado inicial del bus ---\n");
    interpretar_lineas_usb();
    
    pio_usb_configuration_t config = PIO_USB_DEFAULT_CONFIG;
    config.pin_dp = PIN_D_PLUS;
    config.pinout = PIO_USB_PINOUT_DPDM;
    
    printf("\nInicializando PIO-USB Host...\n");
    usb_device = pio_usb_host_init(&config);
    if (!usb_device) {
        printf("ERROR: Falló la inicialización\n");
        return -1;
    }
    printf("✓ Host inicializado\n");
    printf("========================================\n\n");
    
    uint32_t last_print = 0;
    bool was_connected = false;
    
    while (true) {
        pio_usb_host_frame();
        frame_count++;
        
        uint32_t now = to_ms_since_boot(get_absolute_time());
        
        // DETECCIÓN REAL: Solo cuando el stack USB reporta connected
        if (usb_device->connected && !was_connected) {
            printf("\n*** ¡NUEVO DISPOSITIVO USB DETECTADO! ***\n");
            printf("VID: 0x%04X, PID: 0x%04X\n", usb_device->vid, usb_device->pid);
            printf("Class: 0x%02X\n", usb_device->device_class);
            
            if (usb_device->device_class == 0x03) { // Clase HID
                printf("✓ Dispositivo HID detectado (posible teclado)\n");
            }
            
            interpretar_lineas_usb();
            was_connected = true;
            teclado_conectado = true;
            
        } else if (!usb_device->connected && was_connected) {
            printf("\n*** DISPOSITIVO DESCONECTADO ***\n");
            interpretar_lineas_usb();
            was_connected = false;
            teclado_conectado = false;
        }
        
        // Si hay teclado conectado, leer datos
        if (teclado_conectado) {
            for (int i = 0; i < PIO_USB_DEV_EP_CNT; i++) {
                endpoint_t *ep = pio_usb_get_endpoint(usb_device, i);
                if (ep && ep->size > 0 && (ep->ep_num & 0x80)) {
                    uint8_t buffer[8];
                    int len = pio_usb_get_in_data(ep, buffer, sizeof(buffer));
                    
                    static uint8_t last_report[8] = {0};
                    if (len > 0 && memcmp(buffer, last_report, len) != 0) {
                        print_hid_report(usb_device->address, buffer, len);
                        memcpy(last_report, buffer, len);
                    }
                }
            }
        }
        
        // Información periódica (cada 5 segundos)
        if (now - last_print > 5000) {
            printf("\n--- Estado actual (frame %lu, tiempo %lu s) ---\n", 
                   frame_count, now/1000);
            
            if (usb_device->connected) {
                printf("✓ Dispositivo conectado: VID=0x%04X, PID=0x%04X\n", 
                       usb_device->vid, usb_device->pid);
            } else {
                printf("✗ No hay dispositivo conectado\n");
            }
            
            interpretar_lineas_usb();
            printf("----------------------------------------\n");
            
            last_print = now;
        }
        
        sleep_us(100);
    }
    
    return 0;
}