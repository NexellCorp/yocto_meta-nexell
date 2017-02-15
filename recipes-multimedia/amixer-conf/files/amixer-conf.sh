#!/bin/sh

amixer sset 'HPL Output Mixer L_DAC' on;
amixer sset 'HPR Output Mixer R_DAC' on;
amixer sset 'HP DAC' on on;
# HPL 6 -> 0 DB
amixer sset 'HP Driver Gain' 6;
# Digital Volume
amixer sset 'PCM' 116;
# Reset BIAS
amixer sset 'Output Common Mode' "1.65V"
amixer sset 'LDOIN Input Range' '1.8V to 3.6V'
amixer sset 'HPL/HPR Powered Supply' LDOIN

