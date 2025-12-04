/*
 * sig_gen.h
 *
 *  Created on: Nov 11, 2025
 *      Author: mpari
 */

// =============================================================================
// This is a simple signal generator API (uses TIM6 → DMA → DAC1)
// =============================================================================
#ifndef INC_SIGGEN_H_
#define INC_SIGGEN_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// waveform enum
typedef enum {
    SIG_WAVE_SINE = 0,
    SIG_WAVE_SQUARE = 1,
} sig_wave_t;

typedef struct {
	   sig_wave_t waveform;
	   uint32_t freq;
       bool correctForm;
    } GenWave;

// Initialize the generator (Initializes DAC+DMA and configures buffers)
void SigGen_Init(void);

// Start/Stop the timer that clocks the DMA (starts/stops the output)
void SigGen_Start(void);
void SigGen_Stop(void);

// Change both Waveform type and Freq at once (returns false if inputs invalid)
bool SigGen_Set(sig_wave_t wave, uint32_t fout_hz);

// Parse simple CLI commands like
// Valid command: <waveform> <freq>
// Example: "Sine 4000" or "Square 400"
// Returns true on success. On failure, no change is applied
GenWave SigGen_ParseCommand(const char *cmd);

#ifdef __cplusplus
}
#endif
#endif // SIG_GEN_H

