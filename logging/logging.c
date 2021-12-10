#include "logging.h"
#include <stdio.h>
#include <string.h>

// ////////////////////////////////////////////////////////////////////////////
// FARBEN
// ////////////////////////////////////////////////////////////////////////////

// Anzahl der Farben, ohne die default-Farbe.
const int colorCount = 5;
// Alle Farben, ohne die default Farbe.
const char* colors[] = {KYEL, KRED, KGRN, KBLU, KCYN};

const int getColorCount() {
    return colorCount;
}

const char** getColors() {
    return colors;
}

// ////////////////////////////////////////////////////////////////////////////
// LOGGING
// ////////////////////////////////////////////////////////////////////////////

void logger(char* color, char* level, char* message) {
    printf("%s[%s] %s%s\n", color, level, message, KNRM);
}

void logInfo(char* message) {
    logger(KNRM, INFO, message);
}

void logWarning(char* message) {
    logger(KYEL, WARNING, message);
}

void logError(char* message) {
    logger(KRED, ERROR, message);
}

void logCommandStructure() {
    printf("Command must be structured in the following way:\n");
    printf("%s[brainfuckInterpreter]%s [relativeFilePath] %s{-c -r}%s\n", KGRN, KBLU, KCYN, KNRM);
}

void logUnknownOperatorSyntaxError(char unknownOperator) {
    char operator[2];
    operator[0] = unknownOperator;
    operator[1] = '\0';
    char msg[1024];
    msg[0] = '\0';
    strcat(msg, "Encountered unknown operator: ");
    strcat(msg, operator);
    strcat(msg, " Will skip it.");
    logger(KYEL, SYNTAX_ERROR, msg);
}

void logMissingClosingBracket(int position) {
    char pos[12];
    sprintf(pos, "%d", position);
    char msg[1024];
    msg[0] = '\0';
    strcat(msg, "Failed to find closing bracket for bracket at position: ");
    strcat(msg, pos);
    logger(KRED, SYNTAX_ERROR, msg);
}

void logTime(int time) {
    char t[12];
    sprintf(t, "%d", time);
    char msg[1024];
    msg[0] = '\0';
    strcat(msg, "Code has been executed. It took ");
    strcat(msg, t);
    strcat(msg, "s to execute.");
    logger(KNRM, INFO, msg);
}