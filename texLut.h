#ifndef TEXLUT_H
#define TEXLUT_H

#define Rin 0
#define Rin_Amused 0
#define Rin_Surprised 1

typedef enum Background {
    SnowyWoods = 0
} Background;

typedef enum Effect {
    SnowFall = 0
} Effect;

static char* RinPoses[] = {
    "\\GFX\\RIN\\NORMAL\\RINAMU.TIM;1",
    "\\GFX\\RIN\\NORMAL\\RINSUP.TIM;1"
};

static char* Backgrounds[] = {
    "\\GFX\\BG\\SNOWY.TIM;1"
};

static char* Effects[] = {
    "\\GFX\\VFX\\SNOW.TIM;1"
};

#endif