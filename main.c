#include "ANSIescape.h" 
#define BORDERSYM "&"
#define DEBUG 1


typedef struct {
    const int gwidth, gheight; // width and height of the game field
    int winwidth, winheight;
    int px, py, gpx, gpy; // global coordinates (px, py) and game coordinates (x, y)
    int ghx, ghy, gmx, gmy, ghx1, ghx2; // home coords and max coords
    int runcond;
    unsigned char left, right, and, or, xor;
    unsigned char lbuf[33], rbuf[33], abuf[33];
    char *openfield; // pointer to the open field array
    char *colorfield;
    // char *fgcolorfield;
    // char *bgcolorfield;
} field;

/* Function prototypes from main.c */
void outfield(field *face, const char *filename);
void ss_openfield(field *face, int x, int y, char value);
void ss_colorfield(field *face, int x, int y, char value);
void simple_set_1d(field *face, int xypos, char value);
char simple_get(field *face, int x, int y);
char simple_get_1d(field *face, int xypos);
void renderfield(field *face);
void player(field *face);
int gameloop(field *face);
field setfield(int gwidth, int gheight);
void indexify(field *face);
void datacols(field *face, int width);



void outfield(field *face, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }
    for (int i = 0; i < face->gheight; i++) {
        for (int j = 0; j < face->gwidth; j++) {
            fprintf(file, "%c", simple_get(face, j, i));
            if (j < face->gwidth - 1) {
                fprintf(file, "");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void ss_openfield(field *face, int x, int y, char value) {
    if (x >= 0 && x < face->gwidth && y >= 0 && y < face->gheight) {
        face->openfield[y * face->gwidth + x] = value;
    }
}

void ss_colorfield(field *face, int x, int y, char value) {
    if (x >= 0 && x < face->gwidth && y >= 0 && y < face->gheight) {
        face->colorfield[y * face->gwidth + x] = value;
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
  cpos(face->ghx, face->ghy);
  printf("0b%b", face->left);
  cpos(face->ghx, face->ghy - 2);
  printf("px: %d py: %d gpx: %d gpy: %d",
         face->px, face->py,
        //  face->px - face->ghx, face->py - face->ghy);
         face->gpx, face->gpy);
  cpos(face->ghx, face->ghy - 1);
  printf("OF char: %c CF char: %c lbuf: %s addr: %p", simple_get(face, face->gpx, face->gpy), face->colorfield[face->gpy * face->gwidth + face->gpx], face->lbuf, &face->openfield[face->gpy * face->gwidth + face->gpx]);
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
        // printf("\033[48;2;%d;%d;%dm",red,green,blue);
        // bgcolor(0x0000AA);
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
  fgcolor(BLACK);
  bgcolor(YELLOW);

  // cpos(face->ghx1, face->ghy);
  // printf("#%bb", (unsigned char)face->right);
  // cpos(face->ghx1, face->ghy + 1);
  // printf("#%Xx", (unsigned char)face->right );
  // cpos(face->ghx1, face->ghy + 2);
  // printf("#%dd", (unsigned char)face->right);
  // cpos(face->ghx1, face->ghy);

  // cpos(face->ghx2, face->ghy);
  // printf("#%bb", (unsigned char)face->and);
  // cpos(face->ghx2, face->ghy + 1);
  // printf("#%Xx", (unsigned char)face->and );
  // cpos(face->ghx2, face->ghy + 2);
  // printf("#%dd", (unsigned char)face->and);
  // cpos(face->ghx2, face->ghy);

}


void player(field *face) {
  char ch;
  ch = getchar();
  switch (ch) {
  case '1':
    ss_openfield(face, face->px - face->ghx, face->py - face->ghy, '1');
    if ((face->px < face->gmx) | DEBUG)
      face->px++;
    break;
  case '0':
  ss_openfield(face, face->gpx, face->gpy, '0');
    if ((face->px < face->gmx) | DEBUG)
      face->px++;
    break;
  case 'a':
    face->px = face->ghx;
    break;
  case 'k':
    if ((face->py > face->ghy) | DEBUG)
      face->py--;
    break;
  case 'j':
    if ((face->py < face->gmy) | DEBUG)
      face->py++;
    break;
  case 'h':
    if ((face->px > face->ghx) | DEBUG)
      face->px--;
    break;
  case 'l':
    if ((face->px < face->gmx) | DEBUG)
      face->px++;
    break;
  case 'q':
    face->runcond = 0;
    break;
  default:
    break;
  }
  indexify(face);
}


int gameloop(field *face) {
    enable_raw_mode();   // switch terminal into raw mode so input/output behave character-by-character
    while (face->runcond == 1) {
      face->and = face->left & face->right;
      face->or = face->left | face->right;
      face->xor = face->left ^ face->right;
      chome();
      bgcolor(LIGHTCYAN);
      renderfield(face);
      cpos(face->px, face->py);
      fgcolor(BLUE);
      bgcolor(WHITE);
      clearcolor();
      player(face);
    }
    clearcolor();       // reset terminal colors back to defaults
    disable_raw_mode();  // restore terminal settings before exiting
    chome();            // move the cursor to the top-left corner
    free(face->openfield); // free the dynamically allocated openfield array
    free(face->colorfield); // free the dynamically allocated colorfield array
    return face->runcond;
}

field setfield(int gwidth, int gheight) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    field face = {
        .gwidth = gwidth,
        .gheight = gheight,
         //if gameboard is centered 
        .ghx = (w.ws_col / 2) - (face.gwidth / 2),
        .px = (w.ws_col / 2) - (face.gwidth / 2),
        .ghy = (w.ws_row / 2) - (face.gheight / 2),
        .py = (w.ws_row / 2) - (face.gheight / 2),
        .ghx1 = face.ghx + 14,
        .ghx2 = face.ghx + 26,        
        .gmx = face.ghx + (face.gwidth - 1), //May be reassigned in middlecol()
        .gmy = face.ghy + (face.gheight - 1),
        // .gpx = 0,
        // .gpy = 0,
        .winwidth = w.ws_col,
        .winheight = w.ws_row,
        .runcond = 1,
        .left = '\xF4',
        // .lbuf[33] = 
        .right = '\xAF'
        /*
        1: running
        0: stopped by user
        2: ??:?
        */
    };
    printf("gwidth: %d gheight: %d ghx: %d ghy: %d gmx: %d gmy: %d px: %d py: %d gpx: %d gpy: %d\n", face.gwidth, face.gheight, face.ghx, face.ghy, face.gmx, face.gmy, face.px, face.py, face.gpx, face.gpy);
    face.openfield = malloc(sizeof(char) * gwidth * gheight); 
    face.colorfield = malloc(sizeof(char) * gwidth * gheight); 
    // face.fgcolorfield = malloc(sizeof(int) * gwidth * gheight); 
    // face.bgcolorfield = malloc(sizeof(int) * gwidth * gheight); 
    memset(face.colorfield, 0x00, gwidth * gheight);
    memset(face.openfield, ' ', gwidth * gheight);
    return face;
}

void indexify(field *face) {
  face->gpx = face->px - face->ghx;
  face->gpy = face->py - face->ghy;
}

void datacols(field *face, int width) {
    // int midx = face->gwidth / 2;
    // int maxx =  (face->gwidth - width)/2 - 1;
    int col1 = 11;
    int col2 = 25;
    face->gmx = col1 + face->ghx - 1;
    // face->gmx = 
    for (int y = 0; y < face->gheight; y++) {
      for (int i = col1; i < col1 + width; i++) {
      ss_openfield(face, i, y, '_');
      }
      ss_openfield(face, col2, y, '_');
    }
}


  // cpos(face->ghx, face->ghy);
  // printf("#%bb", (unsigned char)face->left);
  // cpos(face->ghx, face->ghy + 1);
  // printf("#%Xx", (unsigned char)face->left );
  // cpos(face->ghx, face->ghy + 2);
  // printf("#%dd", (unsigned char)face->left);
  // cpos(face->ghx, face->ghy);



void quickcopy(field *face, char buf[10], int x, int y) {
  for (int i = 0; i < 10; i++) {
    ss_openfield(face, x + i, y, buf[i]);
  }
}

int main() {
    field face = setfield(37, 5);
    datacols(&face, 3);

    char buf[11];
    snprintf(face.lbuf, 11, "b#%b", (unsigned char)face.left);
    quickcopy(&face, face.lbuf, 0, 0);

    // simpleset(&face, 11, 0, '_');
    outfield(&face, "field.txt");
    gameloop(&face);
    return 0;           // exit the program with success status
}
