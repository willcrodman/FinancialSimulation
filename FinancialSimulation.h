#include <stdbool.h>

// Simulation data structure for fl and nlf
typedef struct financialIdentity {
  double savings;
  double checking;
  double debt;
  double mortgage;
  double debtPaid;
  int yearsWithDebt;
  int yearsRented;
  bool ownsHouse;
} financialIdentity;

struct financialIdentity FLPerson [40];

struct financialIdentity NFLPerson [40];

struct financialIdentity simulationPerson();

void writePerson(FILE *fp, int year, bool financiallyLiterate);

void writeWealth(FILE *fp, struct financialIdentity *person);

void simulateAndOutput(char filename[], bool financiallyLiterate);

void outputLevelII(char filename[], bool financiallyLiterate);

void subCheckingSavings(struct financialIdentity *person, double ammount);

void savingsPlacement(struct financialIdentity *person, double interestRate);

void debt(struct financialIdentity *person, double interestRate, double addlPay);

void rent(struct financialIdentity *person, double rentAmount);

void house(struct financialIdentity *person, double mortgageAmount, double interestRate, int mortgageTerm);

int *simulate(struct financialIdentity *person, double yearlySalary, bool financiallyLiterate);
