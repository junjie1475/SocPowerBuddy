/*
 *  samplers.h
 *  SocPowerBuddy
 *
 *  Created by BitesPotatoBacks on 7/19/22.
 *  Copyright (c) 2022 BitesPotatoBacks.
 */

#ifndef socpwrbud_h
#define socpwrbud_h

#include <Foundation/Foundation.h>
#include <sys/sysctl.h>

/*
 * Extern declarations
 */
enum {
    kIOReportIterOk,
    kIOReportIterFailed,
    kIOReportIterSkipped
};

typedef struct IOReportSubscriptionRef* IOReportSubscriptionRef;
typedef CFDictionaryRef IOReportSampleRef;

extern IOReportSubscriptionRef IOReportCreateSubscription(void* a, CFMutableDictionaryRef desiredChannels, CFMutableDictionaryRef* subbedChannels, uint64_t channel_id, CFTypeRef b);
extern CFDictionaryRef IOReportCreateSamples(IOReportSubscriptionRef iorsub, CFMutableDictionaryRef subbedChannels, CFTypeRef a);
extern CFDictionaryRef IOReportCreateSamplesDelta(CFDictionaryRef prev, CFDictionaryRef current, CFTypeRef a);

extern CFMutableDictionaryRef IOReportCopyChannelsInGroup(NSString*, NSString*, uint64_t, uint64_t, uint64_t);

typedef int (^ioreportiterateblock)(IOReportSampleRef ch);
extern void IOReportIterate(CFDictionaryRef samples, ioreportiterateblock);

extern int IOReportStateGetCount(CFDictionaryRef);
extern uint64_t IOReportStateGetResidency(CFDictionaryRef, int);
extern uint64_t IOReportArrayGetValueAtIndex(CFDictionaryRef, int);
extern long IOReportSimpleGetIntegerValue(CFDictionaryRef, int);
extern NSString* IOReportChannelGetChannelName(CFDictionaryRef);
extern NSString* IOReportChannelGetSubGroup(CFDictionaryRef);
extern NSString* IOReportStateGetNameForIndex(CFDictionaryRef, int);

extern void IOReportMergeChannels(CFMutableDictionaryRef, CFMutableDictionaryRef, CFTypeRef);
extern NSString* IOReportChannelGetGroup(CFDictionaryRef);
/*
 * Typedefs
 */

/* raw data form the ioreport */
typedef struct {
    /* data for Energy Model*/
    IOReportSubscriptionRef pwrsub;
    CFMutableDictionaryRef  pwrsubchn;
    CFMutableDictionaryRef  pwrchn_eng;
    CFMutableDictionaryRef  pwrchn_pmp;

    /* datat for CPU/GPU Stats */
    IOReportSubscriptionRef cpusub;
    CFMutableDictionaryRef  cpusubchn;
    CFMutableDictionaryRef  cpuchn_cpu;
    CFMutableDictionaryRef  cpuchn_gpu;
    
    /* data for CLPC Stats*/
    IOReportSubscriptionRef clpcsub;
    CFMutableDictionaryRef  clpcsubchn;
    CFMutableDictionaryRef  clpcchn;
} iorep_data;

typedef struct {
    NSArray* complex_pwr_channels;
    NSArray* core_pwr_channels;
    
    NSArray* complex_freq_channels;
    NSArray* core_freq_channels;
    
    NSMutableArray* dvfm_states_holder;
    NSArray* dvfm_states;
    
    NSMutableArray* dvfm_states_voltages_holder;
    NSArray* dvfm_states_voltages;
    
    NSMutableArray* cluster_core_counts;
    int gpu_core_count;
    
    NSMutableArray* extra;
} static_data;

typedef struct {
    /* data for freqs, dvfm, and res */
    NSMutableArray* cluster_sums;
    NSMutableArray* cluster_residencies;
    NSMutableArray* cluster_freqs;
    NSMutableArray* cluster_use;

    NSMutableArray* core_sums;
    NSMutableArray* core_residencies;
    NSMutableArray* core_freqs;
    NSMutableArray* core_use;
    
    /* data for power draw */
    NSMutableArray* cluster_pwrs;
    NSMutableArray* core_pwrs;
    
    /* data for (milli)voltage */
    NSMutableArray* cluster_volts;
    NSMutableArray* core_volts;
    
    /* data for instructions and cycles  */
    NSMutableArray* cluster_instrcts_ret;
    NSMutableArray* cluster_instrcts_clk;
//
//    unsigned long package_instrcts_ret;
//    unsigned long package_instrcts_clk;
} variating_data;

/* for cmd opts */
typedef struct cmd_data {
    float        power_measure;
    float        freq_measure;
    const char*  power_measure_un;
    const char*  freq_measure_un;
    
    unsigned int interval;
    int          samples;
    NSArray*     metrics;
    NSArray*     hide_units;
    
    bool  plist;
    FILE * file_out;
} cmd_data;

/* for units/metrics args */
typedef struct {
    /* units */
    bool ecpu;
    bool pcpu;
    bool gpu;
//    bool ane;
    /* metrics */
    bool res;
    bool idle;
    bool freq;
    bool cores;
    bool dvfm;
    bool dvfm_ms;
    bool dvfm_volts;
    bool power;
    bool volts;
    bool intstrcts;
    bool cycles;
} bool_data;


/*
 * Function declarations
 */
void error(int, const char*, ...);
void textOutput(iorep_data*, static_data*, variating_data*, bool_data*, cmd_data*, unsigned int);
void plistOutput(iorep_data*, static_data*, variating_data*, bool_data*, cmd_data*, unsigned int);

void sample(iorep_data*, static_data*, variating_data*, cmd_data*);
void format(static_data*, variating_data*);

void generateDvfmTable(static_data*);
void generateCoreCounts(static_data*);
void generateProcessorName(static_data*);
void generateSiliconCodename(static_data*);
void generateMicroArchs(static_data*);

#endif /* socpwrbud_h */
