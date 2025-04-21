
#include <pico/stdlib.h>
#include "hardware/pwm.h"
#include "hardware/clocks.h"

//Definições buzzer
#define BUZZER_PIN_A 21
#define BUZZER_PIN_B 10


// Frequências das 12 notas da escala cromática na oitava 4
double music_notes[12] = {
    261.63, // C (Dó)
    277.18, // C# / Db
    293.66, // D (Ré)
    311.13, // D# / Eb
    329.63, // E (Mi)
    349.23, // F (Fá)
    369.99, // F# / Gb
    392.00, // G (Sol)
    415.30, // G# / Ab
    440.00, // A (Lá)
    466.16, // A# / Bb
    493.88  // B (Si)
};

// Função PWM para buzzer
void setup_buzzer(uint BUZZER_PIN, uint frequency) {
    // Configura o pino do buzzer para PWM
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    
    // Obtém o slice do pino
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);

    // Calcula o divisor de clock
    // Clock padrão do Raspberry Pi Pico em Hz
    uint clock = 125000000;
    uint divider = clock / (frequency * 4096);
    // Garante que o divisor seja válido
    if (divider < 1) divider = 1;  
    pwm_set_clkdiv(slice_num, divider);

    // Configura o período do PWM
    pwm_set_wrap(slice_num, 4095);

    // Define o duty cycle como 0% (desligado)
    pwm_set_gpio_level(BUZZER_PIN, 0);

    // Não ativa o PWM ainda, apenas configura o slice
    pwm_set_enabled(slice_num, false);
}

void setup_buzzers(uint frequency) {
    setup_buzzer(BUZZER_PIN_A, frequency);
    setup_buzzer(BUZZER_PIN_B, frequency);
}

//Função para ligar buzzer
void start_buzzer(uint BUZZER_PIN, uint frequency) {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM); // Configura o pino como PWM
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);

    // Configura a frequência do PWM
    uint clock = 125000000; // Clock base do sistema
    uint divider = clock / (frequency * 4096);
    if (divider < 1) divider = 1;
    pwm_set_clkdiv(slice_num, divider);

    pwm_set_wrap(slice_num, 4095);         // Resolução do PWM (12 bits)
    pwm_set_gpio_level(BUZZER_PIN, 2048);  // Define o duty cycle (50%)
    pwm_set_enabled(slice_num, true);      // Habilita o PWM
}

void start_buzzers(uint frequency) {
    start_buzzer(BUZZER_PIN_A, frequency);
    start_buzzer(BUZZER_PIN_B, frequency);
}

//Função para desligar buzzer
void stop_buzzer(uint BUZZER_PIN) {
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    // Define o duty cycle como 0 (sem som)
    pwm_set_gpio_level(BUZZER_PIN, 0);
    pwm_set_enabled(slice_num, false);   
}

void stop_buzzers() {
    stop_buzzer(BUZZER_PIN_A);
    stop_buzzer(BUZZER_PIN_B);
}