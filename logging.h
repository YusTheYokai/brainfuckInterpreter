#define KNRM  "\x1B[0m"
#define KYEL  "\x1B[33m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KBLU  "\x1B[34m"
#define KCYN  "\x1B[36m"

#define INFO "INFO"
#define WARNING "WARN"
#define ERROR "ERROR"

void logger(char* color, char* level, char* message);
void logInfo(char* message);
void logWarning(char* message);
void logError(char* message);

void logCommandStructure();
void logUnknownOperatorSyntaxError(char unknownOperator);
void logMissingClosingBracket(int position);
void logTime(int time);