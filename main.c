#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

/* Constants & Enums */

#define MAX_BUFFS 10
#define STAT_COUNT 4

typedef enum {
  STAT_HEALTH = 0,
  STAT_STRENGTH,
  STAT_DEFENSE,
  STAT_SPEED
} StatType;

typedef enum { MOD_ADDITIVE = 0, MOD_MULTIPLICATIVE } ModifierType;

const char *StatNames[] = {"Health", "Strength", "Defense", "Speed"};

/* Structs */

typedef struct {
  StatType type;
  float baseValue;
  float finalValue;
} Stat;

typedef struct {
  StatType affectedStat;
  ModifierType type;
  float value;    /* e.g., 10.0 for +10, 1.2 for +20% */
  float duration; /* in seconds */
  float remainingTime;
  int active; /* 1 if active, 0 if empty/expired */
} Buff;

typedef struct {
  Stat stats[STAT_COUNT];
  Buff activeBuffs[MAX_BUFFS];
  int level;
  int currentXP;
  int xpToNextLevel;
} Player;

/* Function Prototypes */

void InitializePlayer(Player *p);
void CalculateStats(Player *p);
void AddBuff(Player *p, StatType stat, ModifierType type, float value,
             float duration);
void TickBuffs(Player *p, float deltaTime);
void AddXP(Player *p, int amount);
void CheckLevelUp(Player *p);
void PrintStats(const Player *p);
void PrintBuffs(const Player *p);
void PrintMenu(int autoTick);

/* Core Logic */

void InitializePlayer(Player *p) {
  p->level = 1;
  p->currentXP = 0;
  p->xpToNextLevel = 100; /* Initial XP requirement */

  /* Initialize Stats */
  p->stats[STAT_HEALTH].type = STAT_HEALTH;
  p->stats[STAT_HEALTH].baseValue = 100.0f;

  p->stats[STAT_STRENGTH].type = STAT_STRENGTH;
  p->stats[STAT_STRENGTH].baseValue = 10.0f;

  p->stats[STAT_DEFENSE].type = STAT_DEFENSE;
  p->stats[STAT_DEFENSE].baseValue = 5.0f;

  p->stats[STAT_SPEED].type = STAT_SPEED;
  p->stats[STAT_SPEED].baseValue = 1.0f;

  /* Initialize Buffs */
  for (int i = 0; i < MAX_BUFFS; i++) {
    p->activeBuffs[i].active = 0;
  }

  CalculateStats(p);
}

void CalculateStats(Player *p) {
  for (int i = 0; i < STAT_COUNT; i++) {
    float base = p->stats[i].baseValue;
    float additive = 0.0f;
    float multiplier = 1.0f;

    for (int j = 0; j < MAX_BUFFS; j++) {
      if (p->activeBuffs[j].active &&
          p->activeBuffs[j].affectedStat == (StatType)i) {
        if (p->activeBuffs[j].type == MOD_ADDITIVE) {
          additive += p->activeBuffs[j].value;
        } else if (p->activeBuffs[j].type == MOD_MULTIPLICATIVE) {
          multiplier *= p->activeBuffs[j].value;
        }
      }
    }

    p->stats[i].finalValue = (base + additive) * multiplier;
  }
}

void AddBuff(Player *p, StatType stat, ModifierType type, float value,
             float duration) {
  /* Simple logic: Find first empty slot.
     In a real game, you might check for existing buffs of same type to stack or
     refresh.
  */
  for (int i = 0; i < MAX_BUFFS; i++) {
    if (!p->activeBuffs[i].active) {
      p->activeBuffs[i].active = 1;
      p->activeBuffs[i].affectedStat = stat;
      p->activeBuffs[i].type = type;
      p->activeBuffs[i].value = value;
      p->activeBuffs[i].duration = duration;
      p->activeBuffs[i].remainingTime = duration;
      printf(">> Applied Buff: %s %s %.2f for %.1fs\n", StatNames[stat],
             type == MOD_ADDITIVE ? "+" : "x", value, duration);
      CalculateStats(p);
      return;
    }
  }
  printf(">> Buff slots full! Cannot add buff.\n");
}

void TickBuffs(Player *p, float deltaTime) {
  int changed = 0;
  for (int i = 0; i < MAX_BUFFS; i++) {
    if (p->activeBuffs[i].active) {
      p->activeBuffs[i].remainingTime -= deltaTime;
      if (p->activeBuffs[i].remainingTime <= 0) {
        p->activeBuffs[i].active = 0;
        printf(">> Buff Expired: %s\n",
               StatNames[p->activeBuffs[i].affectedStat]);
        changed = 1;
      }
    }
  }
  if (changed) {
    CalculateStats(p);
  }
}

void AddXP(Player *p, int amount) {
  p->currentXP += amount;
  printf(">> Gained %d XP!\n", amount);
  CheckLevelUp(p);
}

void CheckLevelUp(Player *p) {
  while (p->currentXP >= p->xpToNextLevel) {
    p->currentXP -= p->xpToNextLevel;
    p->level++;
    /* XP Curve: Simple quadratic-ish increase */
    p->xpToNextLevel = 100 * p->level * p->level;

    /* Stat Growth */
    p->stats[STAT_HEALTH].baseValue += 10.0f;
    p->stats[STAT_STRENGTH].baseValue += 2.0f;
    p->stats[STAT_DEFENSE].baseValue += 1.0f;

    printf(">> LEVEL UP! Reached Level %d\n", p->level);
    CalculateStats(p);
  }
}

void ClearAllBuffs(Player *p) {
  for (int i = 0; i < MAX_BUFFS; i++) {
    p->activeBuffs[i].active = 0;
  }
  CalculateStats(p);
  printf(">> All buffs cleared.\n");
}

/* UI & Main */

void PrintStats(const Player *p) {
  printf("\n=== PLAYER STATS (Level %d) ===\n", p->level);
  printf("XP: %d / %d\n", p->currentXP, p->xpToNextLevel);
  printf("-------------------------------\n");
  for (int i = 0; i < STAT_COUNT; i++) {
    printf("%-10s: %6.2f (Base: %6.2f)\n", StatNames[i], p->stats[i].finalValue,
           p->stats[i].baseValue);
  }
  printf("===============================\n");
}

void PrintBuffs(const Player *p) {
  printf("Active Buffs:\n");
  int count = 0;
  for (int i = 0; i < MAX_BUFFS; i++) {
    if (p->activeBuffs[i].active) {
      printf(" [%d] %s %s%.2f (%.1fs left)\n", i,
             StatNames[p->activeBuffs[i].affectedStat],
             p->activeBuffs[i].type == MOD_ADDITIVE ? "+" : "x",
             p->activeBuffs[i].value, p->activeBuffs[i].remainingTime);
      count++;
    }
  }
  if (count == 0)
    printf(" (None)\n");
  printf("\n");
}

/* ... (Previous code remains the same up to PrintMenu) ... */

void PrintMenu(int autoTick) {
  printf("1. Show Player Stats\n");
  printf("2. Gain 50 XP\n");
  printf("3. Apply Strength Buff +10 (5s)\n");
  printf("4. Apply Defense Debuff x0.8 (3s)\n");
  printf("5. Apply Speed Buff x1.2 (8s)\n");
  printf("6. Tick Time +1 second\n");
  printf("7. Clear All Buffs\n");
  printf("8. Toggle Autotick (Current: %s)\n", autoTick ? "ON" : "OFF");
  printf("0. Exit\n");
  printf("> ");
}

int main() {
  Player player;
  InitializePlayer(&player);

  int running = 1;
  int autoTick = 0;
  DWORD lastTickTime = GetTickCount();

  printf("Modular Stat & Progression Framework Demo\n");

  while (running) {
    system("cls");
    PrintStats(&player);
    PrintBuffs(&player);
    PrintMenu(autoTick);

    /* Non-blocking Input Loop */
    while (1) {
      /* Check for Autotick */
      if (autoTick) {
        DWORD currentTime = GetTickCount();
        if (currentTime - lastTickTime >= 1000) { // 1 second
          TickBuffs(&player, 1.0f);
          lastTickTime = currentTime;
          break; // Break to redraw UI
        }
      }

      /* Check for Input */
      if (_kbhit()) {
        int key = _getch();
        /* Convert ASCII '0'-'9' to integer */
        int choice = key - '0';

        /* Handle non-digit keys gracefully */
        if (key == 27) {
          choice = 0;
        } // ESC to exit

        printf("%c\n", key); // Echo the key
        Sleep(200);          // Small delay for visual feedback

        switch (choice) {
        case 1:
          break; // Refresh
        case 2:
          AddXP(&player, 50);
          break;
        case 3:
          AddBuff(&player, STAT_STRENGTH, MOD_ADDITIVE, 10.0f, 5.0f);
          break;
        case 4:
          AddBuff(&player, STAT_DEFENSE, MOD_MULTIPLICATIVE, 0.8f, 3.0f);
          break;
        case 5:
          AddBuff(&player, STAT_SPEED, MOD_MULTIPLICATIVE, 1.2f, 8.0f);
          break;
        case 6:
          TickBuffs(&player, 1.0f);
          printf(">> Time advanced by 1s\n");
          break;
        case 7:
          ClearAllBuffs(&player);
          break;
        case 8:
          autoTick = !autoTick;
          lastTickTime = GetTickCount();
          break;
        case 0:
          running = 0;
          printf("Exiting...\n");
          break;
        default:
          break;
        }
        break; // Break inner loop to redraw UI
      }

      Sleep(50); // Prevent CPU hogging
    }
  }

  return 0;
}
