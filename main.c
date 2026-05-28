#include "ANSIescape.h"


#define BORDERSYM "&"
#define DEBUG 0



typedef struct {
    const int gwidth, gheight; // width and height of the game field
    int winwidth, winheight;
    int px, py, gpx, gpy; // global coordinates (px, py) and game coordinates (x, y)
    int ghx, ghy, gmx, gmy; // home cords and max cords
    int runcond;
    char *openfield; // pointer to the open field array
} field;

void renderfield(field *face) {
  clearcolor();
  fgcolor(BLUE);
  bgcolor(BLUE);
  cpos(face->ghx - 1, face->ghy - 2);
  for (int i = 0; i <= face->gwidth + 1; i++) {
    printf(BORDERSYM);
  }
  cpos(face->ghx - 1, face->ghy - 1);
  for (int i = 0; i <= face->gwidth + 1; i++) {
    printf(BORDERSYM);
  }
  for (int y = face->ghy; y < face->ghy + face->gheight; y++) {
    cpos(face->ghx - 1, y);
    printf(BORDERSYM);
    for (int x = 1; x <= face->gwidth; x++) {
      printf(" ");
    }
    printf(BORDERSYM);
  }
  cpos(face->ghx - 1, face->ghy + face->gheight);
  for (int i = 0; i <= face->gwidth + 1; i++) {
    printf(BORDERSYM);
  }
  clearcolor();
  fgcolor(WHITE);
  bgcolor(BLUE);
  cpos(face->ghx, face->ghy - 2);
  printf("X: %d Y: %d fX: %d fY: %d",
         face->px, face->py,
        //  face->px - face->ghx, face->py - face->ghy);
         face->gpx, face->gpy);
  cpos(face->ghx, face->ghy - 1);
  // fflush(stdout);
  for (int row = 0; row < face->gheight; row++) {
    cpos(face->ghx, face->ghy + row);
    for (int col = 0; col < face->gwidth; col++) {
      char cell = face->openfield[row * face->gwidth + col];
      if (cell == ' ') {
        bgcolor(LIGHTCYAN);
        printf(" ");
      } else if (cell == '0') {
        bgcolor(YELLOW);
        fgcolor(RED);
        printf("%c", cell);
      } else if (cell == '1') {
        bgcolor(YELLOW);
        fgcolor(BLUE);
        printf("%c", cell);
      } else if (cell == '_') {
        bgcolor(BLUE);
        fgcolor(BLUE);
        printf("%c", cell);
      } else {
        bgcolor(YELLOW);
        fgcolor(BLACK);
        printf("%c", cell);
      }
    }
  }
}

void simple_set(field *face, int x, int y, char value) {
    if (x >= 0 && x < face->gwidth && y >= 0 && y < face->gheight) {
        face->openfield[y * face->gwidth + x] = value;
    }
}

void simple_set_1d(field *face, int xypos, char value) {
    face->openfield[xypos] = value;
}

char simple_get(field *face, int x, int y) {
    if (x >= 0 && x < face->gwidth && y >= 0 && y < face->gheight) {
        return face->openfield[y * face->gwidth + x];
    }
    return '\0'; // Return null character if out of bounds
}

char simple_get_1d(field *face, int xypos) {
    if (xypos >= 0 && xypos < face->gwidth * face->gheight) {
        return face->openfield[xypos];
    }
    // return face->openfield[xypos];
    return '\0'; // Return null character if out of bounds
}

int player(field *face) {
    return face->runcond;
}

int gameloop(field *face) {
    enable_raw_mode();   // switch terminal into raw mode so input/output behave character-by-character
    int runcond = 1;
    while (runcond == 1) {
      chome();
      bgcolor(LIGHTCYAN);
      renderfield(face);
      cpos(face->px, face->py);
      fgcolor(BLUE);
      bgcolor(WHITE);
      clearcolor();
      // fflush(stdout);
      char ch;
      ch = getchar();
      switch (ch) {
      case '1':
        simple_set(face, face->px - face->ghx, face->py - face->ghy, '1');
        if ((face->px < face->gmx) | DEBUG)
          face->px++;
          face->gpx = face->px - face->ghx;
        break;
      case '0':
        simpleset(face, face->gpx, face->gpy, '0');
        if ((face->px < face->gmx) | DEBUG)
          face->px++;
          face->gpx = face->px - face->ghx;
        break;
      case 'a':
        face->px = face->ghx;
        face->gpx = 0;
        break;
      case 'k':
        if ((face->py > face->ghy) | DEBUG)
          face->py--;
          face->gpy = face->py - face->ghy;
        break;
      case 'j':
        if ((face->py < face->gmy) | DEBUG)
          face->py++;
          face->gpy = face->py - face->ghy;
        break;
      case 'h':
        if ((face->px > face->ghx) | DEBUG)
          face->px--;
          face->gpx = face->px - face->ghx;
        break;
      case 'l':
        if ((face->px < face->gmx) | DEBUG)
          face->px++;
          face->gpx = face->px - face->ghx;
        break;
      case 'q':
        runcond = 0;
        break;
      }

      // runcond -= quick_key();
    }
    clearcolor();       // reset terminal colors back to defaults
    disable_raw_mode();  // restore terminal settings before exiting
    chome();            // move the cursor to the top-left corner
    free(face->openfield); // free the dynamically allocated openfield array
    return runcond;
}

field setfield(int gwidth, int gheight) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    field face = {
        .gwidth = 25,
        .gheight = 15,
         //if gameboard is centered 
        .ghx = (w.ws_col / 2) - (face.gwidth / 2),
        .px = (w.ws_col / 2) - (face.gwidth / 2),
        .ghy = (w.ws_row / 2) - (face.gheight / 2),
        .py = (w.ws_row / 2) - (face.gheight / 2),
        .gmx = face.ghx + (face.gwidth - 1),
        // .gmx = face.ghx + (11),
        .gmy = face.ghy + (face.gheight - 1),
        // .gmy = face.ghy + (11),
        .gpx = 0,
        .gpy = 0,
        .winwidth = w.ws_col,
        .winheight = w.ws_row,
    };
    face.openfield = malloc(sizeof(char) * gwidth * gheight); 
    memset(face.openfield, ' ', gwidth * gheight);
    return face;
}

void middlecol(field *face) {
    int midx = face->gwidth / 2;
    face->gmx = (11) + face->ghx;
     for (int y = 0; y < face->gheight; y++) {
      simple_set(face, midx, y, '_');
    }
}

int main() {
    field face = setfield(25, 15);
    // simpleset(&face, 11, 0, '_');
    middlecol(&face);
    int runcond = gameloop(&face);
    return 0;           // exit the program with success status
}
