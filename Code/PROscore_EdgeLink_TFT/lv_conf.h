/* =========================
 * Core timings
 * ========================= */
#define LV_DEF_REFR_PERIOD                16   /* ok */

/* =========================
 * Color / resolution
 * ========================= */
#define LV_COLOR_DEPTH                    16
#define LV_COLOR_16_SWAP                  0

/* =========================
 * Logging / asserts (save RAM)
 * ========================= */
#define LV_USE_LOG                        0
#define LV_USE_ASSERT_MEM_INTEGRITY       0
#define LV_USE_ASSERT_STYLE               0

/* =========================
 * Theme (keep it minimal)
 * ========================= */
#define LV_USE_THEME_DEFAULT              1
#if LV_USE_THEME_DEFAULT
  #define LV_THEME_DEFAULT_DARK           0
  #define LV_THEME_DEFAULT_GROW           0
  #define LV_THEME_DEFAULT_TRANSITION_TIME 50
#endif

/* =========================
 * Fonts (enable only one small size)
 * ========================= */
#define LV_FONT_MONTSERRAT_14             1
#define LV_FONT_MONTSERRAT_16             0
#define LV_FONT_MONTSERRAT_18             0
#define LV_FONT_MONTSERRAT_20             0
#define LV_FONT_MONTSERRAT_24             0
#define LV_FONT_DEFAULT                   &lv_font_montserrat_14

/* =========================
 * Image cache (tiny)
 * ========================= */
#define LV_IMG_CACHE_DEF_SIZE             1

/* =========================
 * Memory (the big saver)
 * ========================= */
#if LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN
  /* Drop from 96 KB -> 36 KB. You don’t need more unless you’re doing huge canvases. */
  #define LV_MEM_SIZE                     (36 * 1024U)
#endif

/* =========================
 * Draw engine buffers
 * ========================= */
/* Shrink simple layer buffer from 32 KB -> 8 KB */
#define LV_DRAW_SW_LAYER_SIMPLE_BUF_SIZE  (8 * 1024)

/* Optional: disable rotation/sw upscale code paths to shrink */
#define LV_DRAW_SW_ROTATE                 0
#define LV_USE_LARGE_COORD                0
