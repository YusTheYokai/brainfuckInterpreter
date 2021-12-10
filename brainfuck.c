#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "logging/logging.h"

#define ALLOCATION_ERROR_CODE 1
#define SYNTAX_ERROR_CODE 2
#define FILE_NOT_FOUND_ERROR_CODE 3
#define INVALID_ARGUMENTS_ERROR_CODE 4

#define CODE_PARAM "-c"
#define RAINBOW_PARAM "-r"

#define ALLOCATION_SIZE 50

// ////////////////////////////////////////////////////////////////////////////
// TYPEDEFS
// ////////////////////////////////////////////////////////////////////////////

typedef enum operator {
    RIGHT = 62,
    LEFT = 60,
    PLUS = 43,
    MINUS = 45,
    PRINT = 46,
    READ = 44,
    BEGIN_WHILE = 91,
    END_WHILE = 93
} Operator;

// ////////////////////////////////////////////////////////////////////////////
// ALLOCATION
// ////////////////////////////////////////////////////////////////////////////

void checkAllocation(void* pointer) {
    if (pointer == NULL) {
        logError("Speicher konnte nicht zugewiesen werden.");
        exit(ALLOCATION_ERROR_CODE);
    }
}

// ////////////////////////////////////////////////////////////////////////////
// EINGABE
// ////////////////////////////////////////////////////////////////////////////

/*
 * Prüft, ob der übergebene String ein valider Parameter ist.
 */
int validateParameter(const char* toValidate) {
    return strcmp(toValidate, CODE_PARAM) == 0 || strcmp(toValidate, RAINBOW_PARAM) == 0;
}

int validateArguments(int argc, const char* argv[], int* codeFlag, int* rainbowFlag) {
    int invalid = 0;
    if (argc == 1) {
        logError("Invalid amount of arguments.");
        logCommandStructure();
        exit(INVALID_ARGUMENTS_ERROR_CODE);
    } else if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            if (validateParameter(argv[i])) {
                if (strcmp(argv[i], CODE_PARAM) == 0) {
                    *codeFlag = 1;
                } else {
                    *rainbowFlag = 1;
                }
            } else {
                invalid = 1;
                char msg[1024];
                msg[0] = '\0';
                strcat(msg, "Invalid argument: ");
                strcat(msg, argv[i]);
                logger(KRED, ERROR, msg);
            }
        }

        if (invalid) {
            logCommandStructure();
            exit(INVALID_ARGUMENTS_ERROR_CODE);
        }
    }
}

// ////////////////////////////////////////////////////////////////////////////
// READ IN FILE
// ////////////////////////////////////////////////////////////////////////////

/*
 * Prüft, ob der übergebene char ein valider Operator ist.
 */
int isValidOperator(char toValidate) {
    return toValidate == RIGHT || toValidate == LEFT || toValidate == PLUS || toValidate == MINUS ||
            toValidate == PRINT || toValidate == READ || toValidate == BEGIN_WHILE || toValidate == END_WHILE;
}

/*
 * Liest eine Datei anhand des übergebenen Dateinamens ein und gibt
 * den enthaltenen Code ohne Nicht-Brainfuck-Zeichen zurück.
 * @param fileName der Dateiname
 */
char* readFile(const char* fileName) {
    FILE* file = fopen(fileName, "r");

    if (!file) {
        logError("File could not be found!");
        exit(FILE_NOT_FOUND_ERROR_CODE);
    }

    int i = 0;
    char c;
    char* code = malloc(ALLOCATION_SIZE);
    checkAllocation(code);

    while ((c = fgetc(file)) != EOF) {
        // wenn i die Grenze des allokierten Speichers erreicht hat,
        // muss mehr Speicher allokiert werden
        if (i % ALLOCATION_SIZE == ALLOCATION_SIZE - 1) {
            // z.B.                 49 + 1 +              50
            //                 =   100
            code = realloc(code, i + 1 + ALLOCATION_SIZE);
            checkAllocation(code);
        }
        if (isValidOperator(c)) {
            code[i++] = c;
        }
    }
    fclose(file);

    code[i] = '\0';
    return code;
}

// ////////////////////////////////////////////////////////////////////////////
// AUSGABE
// ////////////////////////////////////////////////////////////////////////////

/*
 * Gibt den Code mit Syntax-Highlighting aus.
 * @param code der Code
 */
void print(char* code) {
    for (int i = 0; i < strlen(code); i++) {
        if (code[i] == RIGHT || code[i] == LEFT) {
            printf("%s", KBLU);
        } else if (code[i] == PLUS || code[i] == MINUS) {
            printf("%s", KGRN);
        } else if (code[i] == PRINT || code[i] == READ) {
            printf("%s", KYEL);
        } else if (code[i] == BEGIN_WHILE || code[i] == END_WHILE) {
            printf("%s", KRED);
        }
        printf("%c", code[i]);
        printf("%s", KNRM);
    }
    printf("\n");
}

/*
 * Gibt eine Farbe zurück.
 */
const char* color(int i) {
    return getColors()[i % getColorCount()];
}

// ////////////////////////////////////////////////////////////////////////////
// AUSFÜHREN
// ////////////////////////////////////////////////////////////////////////////

/*
 * Sucht im Code nach der nächsten geschlossenen Klammer ab start.
 * @param code der Code, in welchem nach der Klammer gesucht wird.
 * @param start der inklusive Index, ab dem nach der Klammer gesucht wird.
 * @returns den Index der Klammer oder -1, falls keien Klammer gefunden werden konnte.
 */
int findEndWhile(char* code, int start) {
    int openBrackets = 0;
    while (code[start] != '\0') {
        if (code[start] == END_WHILE && openBrackets == 0) {
            return start;
        } else if (code[start] == END_WHILE) {
            openBrackets--;
        } else if (code[start] == BEGIN_WHILE) {
            openBrackets++;
        }
        start++;
    }
    return -1;
}

/*
 * Führt ein while aus.
 * @param code der Code, welcher im while ausgeführt wird.
 * @param cells die globalen Zellen
 * @param pointer der globale Pointer
 * @param i globaler Zähler
 */
void executeWhile(char* code, int* cells, int* pointer, int* allocationCount, int* i, int* printed, int rainbowFlag);

/*
 * Führt ein Snippet Code aus.
 * @param code der Code
 * @param cells die globalen Zellen
 * @param pointer der globale Pointer
 * @param start Inklusiver Index, ab welchem der Code ausgeführt wird.
 * @param end Exklusiver Index, bis zu welchem der Code ausgefürt wird.
 */
void executeCodeSnippet(char* code, int* cells, int* pointer, int* allocationCount, int start, int end, int* printed, int rainbowFlag) {
    for (int i = start; i < end; i++) {
        if (code[i] == RIGHT) {
            if (*pointer == *allocationCount * ALLOCATION_SIZE - 1) {
                cells = realloc(cells, ++(*allocationCount) * ALLOCATION_SIZE * sizeof(int));
                checkAllocation(code);
                
                for (int j = *pointer + 1; j <= *pointer + ALLOCATION_SIZE; j++) {
                    cells[j] = 0;
                }
            }
            (*pointer)++;
        } else if (code[i] == LEFT) {
            (*pointer)--;
        } else if (code[i] == PLUS) {
            cells[*pointer]++;
        } else if (code[i] == MINUS) {
            cells[*pointer]--;
        } else if (code[i] == PRINT) {
            if (rainbowFlag && isprint(cells[*pointer])) {
                printf("%s%c%s", color((*printed)++), (char) cells[*pointer], KNRM);
            } else {
                printf("%c", (char) cells[*pointer]);
            }
        } else if (code[i] == READ) {
            scanf(" %c", &cells[*pointer]);
        } else if (code[i] == BEGIN_WHILE) {
            executeWhile(code, cells, pointer, allocationCount, &i, printed, rainbowFlag);
        } else if (code[i] == END_WHILE) {
            logWarning("Encountered closing bracket. This should not happen.");
        } else {
            logUnknownOperatorSyntaxError(code[i]);
        }
    }
}

void executeWhile(char* code, int* cells, int* pointer, int* allocationCount, int* i, int* printed, int rainbowFlag) {
    // die passende schließende Klammer finden
    int endWhile = findEndWhile(code, *i + 1);
    if (endWhile == -1) {
        logMissingClosingBracket(*i);
        exit(SYNTAX_ERROR);
    }

    while (cells[*pointer]) {
        executeCodeSnippet(code, cells, pointer, allocationCount, *i + 1, endWhile, printed, rainbowFlag);
    }
    // i um die Differenz der öffnenden und schließenden Klammer erhöhen,
    *i += endWhile - *i;
}

/*
 * Führt Brainfuck Code aus.
 * @param code der auszuführende Code
 */
void executeCode(char* code, int rainbowFlag) {
    int* cells = calloc(ALLOCATION_SIZE, sizeof(int));
    checkAllocation(cells);
    int pointer = 0;
    int allocationCount = 1;
    int printed = 0;
    executeCodeSnippet(code, cells, &pointer, &allocationCount, 0, strlen(code), &printed, rainbowFlag);
    free(cells);
}

// ////////////////////////////////////////////////////////////////////////////
// MAIN
// ////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[]) {
    int codeFlag = 0;
    int rainbowFlag = 0;
    validateArguments(argc, argv, &codeFlag, &rainbowFlag);

    // Datei einlesen
    logInfo("Reading file...");
    char* code = readFile(argv[1]);
    logInfo("File has been read.");

    // Brainfuck Code ausgeben, falls Flag gesetzt wurde
    if (codeFlag) {
        print(code);
    }

    // Brainfuck Code ausführen
    logInfo("Executing read code...\n");
    clock_t executionTime = clock();
    executeCode(code, rainbowFlag);
    printf("\n\n");
    executionTime = clock() - executionTime;

    int time = executionTime / CLOCKS_PER_SEC;

    logTime(time);
    return 0;
}
