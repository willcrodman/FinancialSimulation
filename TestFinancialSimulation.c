#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "FinancialSimulation.c"

void TestDebtFunction() {
  struct financialIdentity person = simulationPerson();
  for(int year = 1; year <= 40; year++) {
    person.checking += 69000 * 0.3;
    person.savings += 69000 * 0.2;
    debt(&person, 0.2, 1);
    if(year % 5 == 0) {
      printPerson(&year, true);
      printWealth(&person);
    }
  }
}

void TestHouseFunction() {
  struct financialIdentity person = simulationPerson();
  person.mortgage = 200000;
  for(int year = 1; year <= 40; year++) {
    person.checking += 69000 * 0.3;
    person.savings += 69000 * 0.2;
    house(&person, 200000, 0.045, 30);
    if(year % 5 == 0) {
      printPerson(&year, true);
      printWealth(&person);
    }
  }
}

void TestSimulations() {
  int* wealth;
  struct financialIdentity person = simulationPerson();
  wealth = simulate(&person, 69000, false);
  for(int year = 1; year <= 40; year++) {
    printf("Year %d wealth: %d\n", year, wealth[year-1]);
  }
}
