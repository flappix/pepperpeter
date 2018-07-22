#include <cstdlib>
#include <iostream>
#include <time.h> 
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

using namespace std;

#define DISTORTION_URI "http://pepper.distortion/PepperDistortion"

typedef enum {
        DISTORTION_INPUT = 0,
        DISTORTION_OUTPUT = 1,
        DISTORTION_TRESHOLD = 2,
        DISTORTION_PEPPER = 3
} PortIndex;

typedef struct {
        // Port buffers
        float* input;
        float* output;
        float* treshold;
        float* pepper;
} Distortion;

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
        Distortion* dist = (Distortion*)malloc(sizeof(Distortion));

        return (LV2_Handle)dist;
}

static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
        Distortion* dist = (Distortion*)instance;

        switch ((PortIndex)port) {
        case DISTORTION_INPUT:
                dist->input = (float*)data;
                break;
        case DISTORTION_OUTPUT:
                dist->output = (float*)data;
                break;
        case DISTORTION_TRESHOLD:
                dist->treshold = (float*)data;
                break;
        case DISTORTION_PEPPER:
                dist->pepper = (float*)data;
                break;
        }
}

static void
activate(LV2_Handle instance)
{
	srand(time(NULL));   // should only be called once
}

static void
run(LV2_Handle instance, uint32_t n_samples)
{
        const Distortion* dist = (const Distortion*)instance;

		float* output   = dist->output;
		float* input   = dist->input;
		float        treshold   = *(dist->treshold);
		float        pepper   = *(dist->pepper);
		
		int cut_start = -1;
		int max_index = 0;
		
		float buffer[n_samples];
		for (uint32_t pos = 0; pos < n_samples; pos++) {
			if (input[pos] > treshold)
			{
				output[pos] = treshold + (input[pos] - treshold)*(1-pepper);
			}
			else if (input[pos] < -treshold)
			{
				output[pos] = -treshold + (input[pos] + treshold)*(1-pepper);
			}
			else
			{
				output[pos] = input[pos];
			}
		}
			
}

static void
deactivate(LV2_Handle instance)
{
}

static void
cleanup(LV2_Handle instance)
{
        free(instance);
}

static const void*
extension_data(const char* uri)
{
        return NULL;
}

static const LV2_Descriptor descriptor = {
        DISTORTION_URI,
        instantiate,
        connect_port,
        activate,
        run,
        deactivate,
        cleanup,
        extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
        switch (index) {
        case 0:  return &descriptor;
        default: return NULL;
        }
}
