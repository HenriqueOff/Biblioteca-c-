#ifndef LOAN_H
#define LOAN_H

#include <string>

struct Loan {
    std::string bookISBN;
    std::string userId;
    std::string loanDate;
    std::string returnDate;
};

#endif /* LOAN_H */

