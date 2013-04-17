/**
 * @file rfm12.h
 * @author Alexander Rössler
 * @brief RFM12 Library
 * @date 28-01-2013
 */
/** @defgroup RFM12 RFM12 Library
 * @ingroup Device
 * @{
 */
#pragma once

#include <ssp.h>
#include <gpio.h>
#include <pincon.h>
#include <sys/types.h>

#define RFM12_MAX_COUNT 5U

typedef enum {
    Rfm12_0 = 0,
    Rfm12_1 = 1,
    Rfm12_2 = 2,
    Rfm12_3 = 3,
    Rfm12_4 = 4
} Rfm12;

typedef enum {
    Rfm12_TxFifo_Enabled = 1,
    Rfm12_TxFifo_Disabled = 0
} Rfm12_TxFifo;
    
typedef enum {
    Rfm12_RxFifo_Enabled = 1,
    Rfm12_RxFifo_Disabled = 0
} Rfm12_RxFifo;

typedef enum {
    Rfm12_FrequencyBand315Mhz = 0b00,
    Rfm12_FrequencyBand433Mhz = 0b01,
    Rfm12_FrequencyBand868Mhz = 0b10,
    Rfm12_FrequencyBand915Mhz = 0b11
} Rfm12_FrequencyBand;

typedef enum {
    Rfm12_CrystalLoadCapacitance8_5pF = 0b0000,
    Rfm12_CrystalLoadCapacitance9_0pF = 0b0001,
    Rfm12_CrystalLoadCapacitance9_5pF = 0b0010,
    Rfm12_CrystalLoadCapacitance10_0pF = 0b0011,
    Rfm12_CrystalLoadCapacitance10_5pF = 0b0100,
    Rfm12_CrystalLoadCapacitance11_0pF = 0b0101,
    Rfm12_CrystalLoadCapacitance11_5pF = 0b0110,
    Rfm12_CrystalLoadCapacitance12_0pF = 0b0111,
    Rfm12_CrystalLoadCapacitance12_5pF = 0b1000,
    Rfm12_CrystalLoadCapacitance13_0pF = 0b1001,
    Rfm12_CrystalLoadCapacitance13_5pF = 0b1010,
    Rfm12_CrystalLoadCapacitance14_0pF = 0b1011,
    Rfm12_CrystalLoadCapacitance14_5pF = 0b1100,
    Rfm12_CrystalLoadCapacitance15_0pF = 0b1101,
    Rfm12_CrystalLoadCapacitance15_5pF = 0b1110,
    Rfm12_CrystalLoadCapacitance16_0pF = 0b1111
} Rfm12_CrystalLoadCapacitance;

typedef enum {
    Rfm12_ReceiverEnabled = 1,
    Rfm12_ReceiverDisabled = 0
} Rfm12_Receiver;

typedef enum {
    Rfm12_BasebandEnabled = 1,
    Rfm12_BasebandDisabled = 0
} Rfm12_Baseband;

typedef enum {
    Rfm12_TransmitterEnabled = 1,
    Rfm12_TransmitterDisabled = 0
} Rfm12_Transmitter;

typedef enum {
    Rfm12_SynthesizerEnabled = 1,
    Rfm12_SynthesizerDisabled = 0
} Rfm12_Synthesizer;

typedef enum {
    Rfm12_OscillatorEnabled = 1,
    Rfm12_OscillatorDisabled = 0
} Rfm12_Oscillator;

typedef enum {
    Rfm12_BrownoutDetectionEnabled = 1,
    Rfm12_BrownoutDetectionDisabled = 0
} Rfm12_BrownoutDetection;

typedef enum {
    Rfm12_WakeupTimerEnabled = 1,
    Rfm12_WakeupTimerDisabled = 0
} Rfm12_WakeupTimer;

typedef enum {
    Rfm12_ClockOutputEnabled = 0,
    Rfm12_ClockOutputDisabled = 1
} Rfm12_ClockOutput;

typedef enum {
    Rfm12_ClockOutputBufferWeak = 0b00,   /** < 2.5MHz */
    Rfm12_ClockOutputBufferMedium = 0b10, /** 3.3MHz */
    Rfm12_ClockOutputBufferStrong = 0b11  /** 5 to 10MHz */
} Rfm12_ClockOutputBuffer;

typedef enum {
    Rfm12_OsciallatorLowPowerMode1msStartup = 0,
    Rfm12_OsciallatorLowPowerMode2msStartup = 1
} Rfm12_OscillatorLowPowerMode;

typedef enum {
    Rfm12_PhaseDetectorDelayDisabled = 0,
    Rfm12_PhaseDetectorDelayEnabled = 1
} Rfm12_PhaseDetectorDelay;

typedef enum {
    Rfm12_PllDitheringEnabled = 0,
    Rfm12_PllDitheringDisabled = 1
} Rfm12_PllDithering;

typedef enum {
    Rfm12_PllBandwith1 = 0x10,
    Rfm12_PllBandwith2 = 0x11
} Rfm12_PllBandwith;

typedef enum {
    Rfm12_Clock1MHz = 0b000,
    Rfm12_Clock1_25MHz = 0b001,
    Rfm12_Clock1_6MHz = 0b010,
    Rfm12_Clock2MHz = 0b011,
    Rfm12_Clock2_5MHz = 0b100,
    Rfm12_Clock3_3MHz = 0b101,
    Rfm12_Clock5MHz = 0b110,
    Rfm12_Clock10MHz = 0b111
} Rfm12_Clock;

typedef enum {
    Rfm12_DataPinInt = 0,
    Rfm12_DataPinVdi = 1
} Rfm12_DataPin;

typedef enum {
    Rfm12_ValidDataIndicatorResponseFast = 0b00,
    Rfm12_ValidDataIndicatorResponseMedium = 0b01,
    Rfm12_ValidDataIndicatorResponseSlow = 0b10,
    Rfm12_ValidDataIndicatorResponseAlwaysOn = 0b11
} Rfm12_ValidDataIndicatorResponse;

typedef enum {
    Rfm12_ReceiverBasebandBandwidth400kHz = 0b001,
    Rfm12_ReceiverBasebandBandwidth340kHz = 0b010,
    Rfm12_ReceiverBasebandBandwidth270kHz = 0b011,
    Rfm12_ReceiverBasebandBandwidth200kHz = 0b100,
    Rfm12_ReceiverBasebandBandwidth134kHz = 0b101,
    Rfm12_ReceiverBasebandBandwidth67kHz = 0b110
} Rfm12_ReceiverBasebandBandwidth;

typedef enum {
    Rfm12_LnaGain0dB = 0b00,
    Rfm12_LnaGain6dB = 0b01,
    Rfm12_LnaGain14dB = 0b10,
    Rfm12_LnaGain20dB = 0b11
} Rfm12_LnaGain;

typedef enum {
    Rfm12_RssiDetectorThreshold103dB = 0b000,
    Rfm12_RssiDetectorThreshold97dB = 0b001,
    Rfm12_RssiDetectorThreshold91dB = 0b010,
    Rfm12_RssiDetectorThreshold85dB = 0b011,
    Rfm12_RssiDetectorThreshold79dB = 0b100,
    Rfm12_RssiDetectorThreshold73dB = 0b101
} Rfm12_RssiDetectorThreshold;

typedef enum {
    Rfm12_AutoLockEnabled = 1,
    Rfm12_AutoLockDisabled = 0
} Rfm12_AutoLock;

typedef enum {
    Rfm12_ManualLockSlow = 0,
    Rfm12_ManualLockFast = 1
} Rfm12_ManualLock;

typedef enum {
    Rfm12_DigitalFilter = 0,
    Rfm12_AnalogRcFilter = 1
} Rfm12_Filter;

typedef enum {
    Rfm12_DqdThresholdAlwayOn = 0b000,
    Rfm12_DqdThresholdMedium = 0b100,
    Rfm12_DqdThresholdMaximum = 0b111
} Rfm12_DqdThreshold;

/** This bit is undocumented, however it is necessary to use OOK modulation/demodulation */
typedef enum {
    Rfm12_OokModulationEnabled = 0,
    Rfm12_OokModulationDisabled = 1
} Rfm12_OokModulation;

typedef enum {
    Rfm12_FifoInterruptLevelBit = 0b0001,
    Rfm12_FifoInterruptLevelByte = 0b1000
} Rfm12_FifoInterruptLevel;

typedef enum {
    Rfm12_SyncPatternLength2Byte = 0,
    Rfm12_SyncPatternLength1Byte = 1
} Rfm12_SyncPatternLength;

typedef enum {
    Rfm12_FifoFillConditionAlways = 1,
    Rfm12_FifoFillConditionSynchronPattern = 0
} Rfm12_FifoFillCondition;

typedef enum {
    Rfm12_FifoFillModeClearAndLock = 0,
    Rfm12_FifoFillModeRelease = 1
} Rfm12_FifoFillMode;

typedef enum {
    Rfm12_SensitiveResetEnabled = 1,
    Rfm12_SensitiveResetDisabled = 0
} Rfm12_SensitiveReset;

typedef enum {
    Rfm12_AfcAutoModeOff = 0b00,
    Rfm12_AfcAutoModeStartup = 0b01,
    Rfm12_AfcAutoModeVdiLow = 0b10,
    Rfm12_AfcAutoModeAlways = 0b11
} Rfm12_AfcAutoMode;

typedef enum {
    Rfm12_AfcRangeLimitUnlimited = 0b00,    /** -64delta f, +62delta f */
    Rfm12_AfcRangeLimitWide = 0b01,         /** -16delta f, +15delta f */
    Rfm12_AfcRangeLimitMedium = 0b10,       /** -8detla f, +7delta f */
    Rfm12_AfcRangeLimitSmall = 0b11         /** -4delta f, +3delta f */
} Rfm12_AfcRangeLimit;

typedef enum {
    Rfm12_AfcStrobeEdgeEnabled = 1,
    Rfm12_AfcStrobeEdgeDisabled = 0
} Rfm12_AfcStrobeEdge;

typedef enum {
    Rfm12_AfcFineModeEnabled = 1,
    Rfm12_AfcFineModeDisabled = 0
} Rfm12_AfcFineMode;

typedef enum {
    Rfm12_AfcOffsetEnabled = 1,
    Rfm12_AfcOffsetDisabled = 0
} Rfm12_AfcOffset;

typedef enum {
    Rfm12_AfcEnabled = 1,
    Rfm12_AfcDisabled = 0
} Rfm12_Afc;

typedef enum {
    Rfm12_ModulationPolarityNormal = 0,
    Rfm12_ModulationPolarityInverted = 1
} Rfm12_ModulationPolarity;

typedef enum {
    Rfm12_FrequencyDeviation15kHz = 0b0000,
    Rfm12_FrequencyDeviation30kHz = 0b0001,
    Rfm12_FrequencyDeviation45kHz = 0b0010,
    Rfm12_FrequencyDeviation60kHz = 0b0011,
    Rfm12_FrequencyDeviation75kHz = 0b0100,
    Rfm12_FrequencyDeviation90kHz = 0b0101,
    Rfm12_FrequencyDeviation105kHz = 0b0110,
    Rfm12_FrequencyDeviation120kHz = 0b0111,
    Rfm12_FrequencyDeviation135kHz = 0b1000,
    Rfm12_FrequencyDeviation150kHz = 0b1001,
    Rfm12_FrequencyDeviation165kHz = 0b1010,
    Rfm12_FrequencyDeviation180kHz = 0b1011,
    Rfm12_FrequencyDeviation195kHz = 0b1100,
    Rfm12_FrequencyDeviation210kHz = 0b1101,
    Rfm12_FrequencyDeviation225kHz = 0b1110,
    Rfm12_FrequencyDeviation240kHz = 0b1111
} Rfm12_FrequencyDeviation;

typedef enum {
    Rfm12_RelativeOutputPower0dB = 0b000,
    Rfm12_RelativeOutputPower2_5dB = 0b001,
    Rfm12_RelativeOutputPower5dB = 0b010,
    Rfm12_RelativeOutputPower7_5dB = 0b011,
    Rfm12_RelativeOutputPower10dB = 0b100,
    Rfm12_RelativeOutputPower12_5dB = 0b101,
    Rfm12_RelativeOutputPower15dB = 0b110,
    Rfm12_RelativeOutputPower17_5dB = 0b111
} Rfm12_RelativeOutputPower;

typedef enum {
    Rfm12_CyclicWakeupEnabled = 1,
    Rfm12_CyclicWakeupDisabled = 0
} Rfm12_CyclicWakeup;

typedef enum {
    Rfm12_ModeNone = 0,
    Rfm12_ModeSender = 1,
    Rfm12_ModeReceiver = 2
} Rfm12_Mode;

/** Here is the place for the status bits */

int8 Rfm12_initialize(Rfm12 id, Ssp ssp, Gpio_Pair selPair, Gpio_Pair dataPair);

void Rfm12_prepareOokSending(Rfm12 id, Rfm12_FrequencyBand frequencyBand, float frequency, uint32 dataRate);
void Rfm12_prepareOokReceiving(Rfm12 id, Rfm12_FrequencyBand frequencyBand, float frequency, uint32 dataRate);

void Rfm12_ookSet(Rfm12 id);
void Rfm12_ookClear(Rfm12 id);
void Rfm12_ookToggle(Rfm12 id);

void Rfm12_setBaseConfig(Rfm12 id,
                        Rfm12_TxFifo txFifoEnable, 
                        Rfm12_RxFifo rxFifoEnable, 
                        Rfm12_FrequencyBand frequencyBand, 
                        Rfm12_CrystalLoadCapacitance loadCapacitance);

void Rfm12_setPowerManagement(Rfm12 id,
                             Rfm12_Receiver receiverEnable,
                             Rfm12_Baseband basebandEnable,
                             Rfm12_Transmitter transmitterEnable,
                             Rfm12_Synthesizer synthesizerEnable,
                             Rfm12_Oscillator xtalEnable,
                             Rfm12_BrownoutDetection brownoutEnable,
                             Rfm12_WakeupTimer wakeupEnable,
                             Rfm12_ClockOutput clockEnable);

void Rfm12_setClockGenerator(Rfm12 id,
                            Rfm12_ClockOutputBuffer outputBuffer,
                            Rfm12_OscillatorLowPowerMode lowPowerXtal,
                            Rfm12_PhaseDetectorDelay phaseDelay,
                            Rfm12_PllDithering ditheringEnable,
                            Rfm12_PllBandwith bandwith);

void Rfm12_setLowBatteryDetectorAndClockDivider(Rfm12 id,
                                               Rfm12_Clock clock,
                                               uint16 thresholdVoltage);

void Rfm12_setFrequency(Rfm12 id, Rfm12_FrequencyBand frequencyBand, float frequency);

void Rfm12_setDataRate(Rfm12 id, uint32 baudrate);

void Rfm12_setReceiverControl(Rfm12 id,
                             Rfm12_DataPin pinIntVdi,
                             Rfm12_ValidDataIndicatorResponse validDataIndicatorResponse,
                             Rfm12_ReceiverBasebandBandwidth receiverBasebandBandwidth,
                             Rfm12_LnaGain lnaGainSelect,
                             Rfm12_RssiDetectorThreshold rssiDetectorThreshold);
                             
void Rfm12_setSynchronPattern(Rfm12 id, uint8 pattern);

void Rfm12_setDataFilter(Rfm12 id,
                        Rfm12_AutoLock autoLock,
                        Rfm12_ManualLock manualLock,
                        Rfm12_Filter filter,
                        Rfm12_DqdThreshold dqdThreshold,
                        Rfm12_OokModulation ookModulationEnable);

void Rfm12_setFifoAndResetMode(Rfm12 id,
                              Rfm12_FifoInterruptLevel fifoInterruptLevel,
                              Rfm12_SyncPatternLength syncPatternLength,
                              Rfm12_FifoFillCondition alwayFill,
                              Rfm12_FifoFillMode fifoFill,
                              Rfm12_SensitiveReset sensResetEnable);

void Rfm12_setAutomaticFrequencyControl(Rfm12 id,
                                       Rfm12_AfcAutoMode autoMode,
                                       Rfm12_AfcRangeLimit rangeLimit,
                                       Rfm12_AfcStrobeEdge strobeEdgeEnable,
                                       Rfm12_AfcFineMode fineModeEnable,
                                       Rfm12_AfcOffset afcOffsetEnable,
                                       Rfm12_Afc afcEnable);
void Rfm12_setTxConfiguration(Rfm12 id,
                             Rfm12_ModulationPolarity modulationPolarity,
                             Rfm12_FrequencyDeviation frequencyDeviation,
                             Rfm12_RelativeOutputPower relativeOutputPower);
                             
void Rfm12_setWakeUpTimer(Rfm12 id, uint8 radix, uint8 mantissa);

void Rfm12_setLowDutyCycle(Rfm12 id,
                          uint8 duration,
                          Rfm12_CyclicWakeup cyclicWakeupEnable);

void Rfm12_softwareReset(Rfm12 id);

uint16 Rfm12_readStatus(Rfm12 id);

char Rfm12_read(Rfm12 id);
void Rfm12_write(Rfm12 id, char byte);


/**
 * @}
 */
