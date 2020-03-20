#include <stdio.h>
#include <math.h>

int main(){

    double sample_rate = 44100.0;

    double duration = 5.0;

    int nb_samples = sample_rate * duration;

    short* samples = new short[nb_samples];

    double tincr = 2 * M_PI * 440.0 / sample_rate;
    double tincr800 = 2 * M_PI * 800.0 / sample_rate;
    double tincr300 = 2 * M_PI * 300.0 / sample_rate;
    double tincr880 = 2 * M_PI * 880.0 / sample_rate;

    double angle = 0;
    double angle800 = 0;
    double angle300 = 0;
    double angle880 = 0;

    short* tempSamples = samples;

    for (int i = 0; i < nb_samples; i++) {

        float amplitude440 = sin(angle);
        float amplitude800 = 0.25 * sin(angle800);
        float amplitude300 = 0.25 * sin(angle300);
        float amplitude880 = 0.5 * sin(angle880);

        float amplitude = (amplitude440 + amplitude800 + amplitude300 + amplitude880) / 2;

        *tempSamples = (int)(amplitude * 32767);

        tempSamples += 1;

        angle += tincr;
        angle800 += tincr800;
        angle300 += tincr300;
        angle880 += tincr880;

    }

    

    //Write To PCM File
    
    FILE* pFILE = fopen("440hzmix.pcm", "w");
    fwrite(samples, sizeof(short), nb_samples, pFILE);
    fclose(pFILE);

    delete[] samples;


    return 0;
}
