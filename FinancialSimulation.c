#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "FinancialSimulation.h"

struct financialIdentity FLPerson [40];
struct financialIdentity NFLPerson [40];

struct financialIdentity simulationPerson() {
  struct financialIdentity person = {.savings = 5000.0, .checking = 0.0, .debt = 52500.0,
    .mortgage = 0.0, .debtPaid = 0.0, .yearsWithDebt = 0,
    .yearsRented = 0, .ownsHouse = false};
  return person;
}

void writePerson(FILE *fp, int year, bool financiallyLiterate) {
  char s1[30] = "Financial ";
  char* s2;
  char s3[30] = " person - Simulation year ";
  if (financiallyLiterate) {
    s2 = "literate";
  } else {
    s2 = "illiterate";
  }
  fprintf(fp, "%s%s%s%d\n", s1, s2, s3, year);
  for(int i = 0; i < 83; i++) { fprintf(fp, "_"); }
  fprintf(fp, "\n");
}

void writeWealth(FILE *fp, struct financialIdentity *person) {
  fprintf(fp, "Years with debt: %d   |   ", person->yearsWithDebt);
  fprintf(fp, "Years rented: %d   |   ", person->yearsRented);
  fprintf(fp, "Owns House: %s\n", person->ownsHouse ? "Yes" : "No");
  for(int i = 0; i < 83; i++) { fprintf(fp, "_"); }
  fprintf(fp, "\n");
  fprintf(fp, "%-20s%-21s%-21s%-21s\n", "ACCOUNT", "ASSET", "DEBT", "EXONERATED DEBT");
  for(int i = 0; i < 83; i++) { fprintf(fp, "_"); }
  fprintf(fp, "\n");
  fprintf(fp, "%-20s|%20lf|%20s|%20s\n", "Savings", person->savings, "", "");
  fprintf(fp, "%-20s|%20lf|%20s|%20s\n", "Checking", person->checking, "", "");
  fprintf(fp, "%-20s|%20s|%20lf|%20s\n", "Loans", "", person->debt, "");
  fprintf(fp, "%-20s|%20s|%20lf|%20s\n", "Mortgage", "", person->mortgage, "");
  fprintf(fp, "%-20s|%20s|%20s|%20lf\n", "Debt Paid", "", "", person->debtPaid);
  for(int i = 0; i < 83; i++) { fprintf(fp, "_"); }
  fprintf(fp, "\n");
  for(int i = 0; i < 83; i++) { fprintf(fp, "_"); }
  fprintf(fp, "\n");
  fprintf(fp, "%-20s|%20lf|%20lf|%20lf\n\n\n", "TOTAL",
   (person->savings + person->checking), (person->debt + person->mortgage), person->debtPaid);
}

void simulateAndOutput(char filename[], bool financiallyLiterate) {
  struct financialIdentity person = simulationPerson();
  int *output;
  FILE *fp;
  output = simulate(&person, 69000, financiallyLiterate);
  fp = fopen(filename, "w");
  for(int i = 0; i < 40; i++) { fprintf(fp, "%d\n", output[i]); }
  fclose(fp);
}

void outputLevelII(char filename[], bool financiallyLiterate) {
  FILE *fp;
  fp = fopen(filename, "w");
  for(int i = 0; i < 40; i++) {
      writePerson(fp, i + 1, financiallyLiterate);
      if(financiallyLiterate) {
        writeWealth(fp, &FLPerson[i]);
      } else {
        writeWealth(fp, &NFLPerson[i]);
      }
   }
  fclose(fp);
}

void subCheckingSavings(struct financialIdentity *person, double ammount) {
  if(person->checking >= ammount) {
    person->checking -= ammount;
  } else {
    double difference = ammount - person->checking;
    person->checking = 0.0;
    person->savings -= difference;
  }
}

void savingsPlacement(struct financialIdentity *person, double interestRate) {
  person->savings = person->savings * (1 + interestRate);
}

void debt(struct financialIdentity *person, double interestRate, double addlPay) {
   for(int i = 0; i < 12; ++i) {
     double pay = (person->debt * 0.03) + addlPay;
     if(person->debt < pay) {
       subCheckingSavings(person, person->debt);
       person->debtPaid += person->debt;
       person->debt = 0;
     } else if(person->debt > 0) {
       subCheckingSavings(person, pay);
       person->debtPaid += pay;
       person->debt -= pay;

     }
    }
    person->debt *= 1 + interestRate;
    person->yearsWithDebt++;
}

void rent(struct financialIdentity *person, double rentAmount) {
  for(int i = 0; i < 12; i++) { subCheckingSavings(person, rentAmount); }
  person->yearsRented++;
}

void house(struct financialIdentity *person, double mortgageAmount, double interestRate, int mortgageTerm) {
     double n, i, d, p;
     n = (double)mortgageTerm * 12.0;
     i = interestRate / 12.0;
     d = (pow((1 + i), n) - 1) / (i * pow((1 + i), n));
     p = mortgageAmount / d;
     for(int j = 0; j < 12; j++) {
       if(person->mortgage < p) {
         subCheckingSavings(person, person->mortgage);
         person->mortgage = 0;
       } else if(person->mortgage > 0) {
         subCheckingSavings(person, p);
         person->mortgage -= p;
       }
     }
     person->mortgage *= 1 + interestRate;
}

int *simulate(struct financialIdentity *person, double yearlySalary, bool financiallyLiterate) {
  static int wealth[40];
  double housePrice = 210000.0;
  double downPayment = (financiallyLiterate ? 0.2 : 0.05) * housePrice;
  for(int i = 0; i <= 40; i++) {
       wealth[i] = (int)(person->checking + person->savings - person->debt - person->mortgage);
       if(financiallyLiterate) {
           FLPerson[i] = *person;
       } else {
           NFLPerson[i] = *person;
       }
       person->checking += yearlySalary * 0.3;
       person->savings += yearlySalary * 0.2;
       savingsPlacement(person, financiallyLiterate ? 0.07 : 0.01);
       if(person->debt > 0) { debt(person, 0.2, (financiallyLiterate ? 100.0 : 1.0)); }
       if(person->ownsHouse == false && person->savings >= downPayment) {
         person->ownsHouse = true;
         person->mortgage = housePrice - downPayment;
       }
       if(person->ownsHouse == false) {
         rent(person, 950.0);
       } else if(person->mortgage > 0) {
         house(person, housePrice - downPayment, (financiallyLiterate ? 0.045 : 0.05), 30);
       }
   }
   return wealth;
}

int main(int argc, char *argv[]) {
  simulateAndOutput(argv[1], true);
  simulateAndOutput(argv[2], false);
  if(argc == 5) {
    outputLevelII(argv[3], true);
    outputLevelII(argv[4], false);
  }
  return 0;
}
