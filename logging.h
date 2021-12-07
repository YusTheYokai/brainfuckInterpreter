#define KNRM  "\x1B[0m"
#define KYEL  "\x1B[33m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KBLU  "\x1B[34m"
#define KCYN  "\x1B[36m"

#define INFO "INFO"
#define WARNING "WARN"
#define ERROR "ERROR"

/*
 * Gibt die Anzahl der Farben, ohne die default-Farbe zurück.
 */
const int getColorCount();

/*
 * Gibt die Farben, ohne die default-Farbe zurück.
 */
const char** getColors();


/*
 * Loggt in die Konsole.
 * @param color Farbe, in welcher geschrieben wird
 * @param level Loglevel
 * @param message Nachricht, welche geloggt wird
 */
void logger(char* color, char* level, char* message);

/*
 * Loggt einen Info-Log in die Konsole.
 * @param message Nachricht, welche geloggt wird
 */
void logInfo(char* message);

/*
 * Loggt einen Warning-Log in die Konsole.
 * @param message Nachricht, welche geloggt wird
 */
void logWarning(char* message);

/*
 * Loggt einen Error-Log in die Konsole.
 * @param message Nachricht, welche geloggt wird
 */
void logError(char* message);


/*
 * Loggt den Befehlsaufbau.
 */
void logCommandStructure();

/*
 * Loggt, für wenn das Programm auf einen unbekannten Operator trifft.
 * @param unknownOperator der unbekannte Operator
 */
void logUnknownOperatorSyntaxError(char unknownOperator);

/*
 * Loggt, für wenn das Programm eine schließende Klammer nicht finden kann.
 * @param position die Position der öffnenden Klammer, zu welcher keine schließende gefunden werden konnte.
 */
void logMissingClosingBracket(int position);

/*
 * Loggt die zum Ausführen gebrauchte Zeit.
 * @param time Anzahl an Sekunden
 */
void logTime(int time);