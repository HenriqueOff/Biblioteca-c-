#include <iostream>
#include <vector>
#include <regex>
#include <cctype>
#include <sstream>
#include "Book.h"
#include "User.h"
#include "Loan.h"
#include "AVLTree.h"
#include <locale.h>

using namespace std;

// Função para validar se a data está no formato DD/MM/AAAA
bool validarData(const string& data) {
    regex pattern("\\d{2}/\\d{2}/\\d{4}");
    if (!regex_match(data, pattern)) {
        return false;
    }

    istringstream iss(data);
    string dia, mes, ano;
    getline(iss, dia, '/');
    getline(iss, mes, '/');
    getline(iss, ano);

    int diaInt = stoi(dia);
    int mesInt = stoi(mes);
    int anoInt = stoi(ano);

    if (mesInt < 1 || mesInt > 12) {
        return false;
    }

    int diasNoMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Verifica ano bissexto
    if ((anoInt % 4 == 0 && anoInt % 100 != 0) || (anoInt % 400 == 0)) {
        diasNoMes[1] = 29;
    }

    if (diaInt < 1 || diaInt > diasNoMes[mesInt - 1]) {
        return false;
    }

    return true;
}

// Função para ler uma entrada não vazia
void lerEntradaNaoVazia(string& entrada, const string& mensagem) {
    do {
        cout << mensagem;
        getline(cin, entrada);
        if (entrada.empty()) {
            cout << "Entrada não pode ser vazia. Tente novamente." << endl;
        }
    } while (entrada.empty());
}

// Função para validar se a entrada contém apenas números
bool validarNumero(const string& entrada) {
    return !entrada.empty() && all_of(entrada.begin(), entrada.end(), ::isdigit);
}

// Função para ler uma entrada numérica
void lerEntradaNumerica(string& entrada, const string& mensagem) {
    do {
        cout << mensagem;
        getline(cin, entrada);
        if (!validarNumero(entrada)) {
            cout << "Entrada inválida. Digite apenas números." << endl;
        }
    } while (!validarNumero(entrada));
}

// Função para ler uma entrada numérica inteira
void lerEntradaNumericaInt(int& entrada, const string& mensagem) {
    string temp;
    do {
        cout << mensagem;
        getline(cin, temp);
        try {
            entrada = stoi(temp);
            break;
        } catch (invalid_argument& e) {
            cout << "Entrada inválida. Digite apenas números inteiros." << endl;
        }
    } while (true);
}

// Função para comparar datas no formato DD/MM/AAAA
bool compararDatas(const string& data1, const string& data2) {
    int dia1, mes1, ano1;
    int dia2, mes2, ano2;
    sscanf(data1.c_str(), "%d/%d/%d", &dia1, &mes1, &ano1);
    sscanf(data2.c_str(), "%d/%d/%d", &dia2, &mes2, &ano2);

    if (ano1 != ano2) {
        return ano1 < ano2;
    }
    if (mes1 != mes2) {
        return mes1 < mes2;
    }
    return dia1 < dia2;
}

int main() {
    AVLTree<Book> bookTree;
    AVLTree<User> userTree;
    vector<Loan> loanList;
    int choice;

    do {
        cout << "Menu:" << endl;
        cout << "1. Adicionar Livro" << endl;
        cout << "2. Remover Livro" << endl;
        cout << "3. Buscar Livro" << endl;
        cout << "4. Listar Livros" << endl;
        cout << "5. Adicionar Usuario" << endl;
        cout << "6. Remover Usuario" << endl;
        cout << "7. Buscar Usuario" << endl;
        cout << "8. Listar Usuarios" << endl;
        cout << "9. Registrar Emprestimo" << endl;
        cout << "10. Registrar Devolucao" << endl;
        cout << "11. Listar Emprestimos" << endl;
        cout << "0. Sair" << endl;
        cout << "Escolha uma opcao: ";
        cin >> choice;
        cin.ignore(); // Ignorar o '\n' que fica no buffer após a leitura do int

        switch (choice) {
            case 1: {
                Book book;
                lerEntradaNumerica(book.isbn, "ISBN: ");
                lerEntradaNaoVazia(book.title, "Titulo: ");
                lerEntradaNaoVazia(book.author, "Autor: ");
                lerEntradaNumericaInt(book.pages, "Numero de Paginas: ");
                bookTree.insert(book.isbn, book);
                break;
            }
            case 2: {
                string isbn;
                lerEntradaNumerica(isbn, "ISBN do livro a ser removido: ");
                bookTree.remove(isbn);
                break;
            }
            case 3: {
                string isbn;
                lerEntradaNumerica(isbn, "ISBN do livro a ser buscado: ");
                Book* book = bookTree.search(isbn);
                if (book != nullptr) {
                    cout << "Titulo: " << book->title << ", Autor: " << book->author << ", Paginas: " << book->pages << endl;
                } else {
                    cout << "Livro nao encontrado." << endl;
                }
                break;
            }
            case 4: {
                bookTree.displayInOrder();
                break;
            }
            case 5: {
                User user;
                lerEntradaNumerica(user.userId, "ID do Usuario: ");
                lerEntradaNaoVazia(user.name, "Nome: ");
                lerEntradaNaoVazia(user.contactInfo, "Dados de Contato: ");
                userTree.insert(user.userId, user);
                break;
            }
            case 6: {
                string userId;
                lerEntradaNumerica(userId, "ID do usuario a ser removido: ");
                userTree.remove(userId);
                break;
            }
            case 7: {
                string userId;
                lerEntradaNumerica(userId, "ID do usuario a ser buscado: ");
                User* user = userTree.search(userId);
                if (user != nullptr) {
                    cout << "Nome: " << user->name << ", Contato: " << user->contactInfo << endl;
                } else {
                    cout << "Usuario nao encontrado." << endl;
                }
                break;
            }
            case 8: {
                userTree.displayInOrder();
                break;
            }
            case 9: {
                Loan loan;
                lerEntradaNumerica(loan.bookISBN, "ISBN do Livro: ");
                lerEntradaNumerica(loan.userId, "ID do Usuario: ");

                // Verifica se o livro existe
                if (bookTree.search(loan.bookISBN) == nullptr) {
                    cout << "Livro não encontrado. Empréstimo não registrado." << endl;
                    break;
                }

                // Verifica se o usuário existe
                if (userTree.search(loan.userId) == nullptr) {
                    cout << "Usuário não encontrado. Empréstimo não registrado." << endl;
                    break;
                }

                do {
                    lerEntradaNaoVazia(loan.loanDate, "Data de Emprestimo (DD/MM/AAAA): ");
                    if (!validarData(loan.loanDate)) {
                        cout << "Data inválida. Tente novamente." << endl;
                    }
                } while (!validarData(loan.loanDate));
                do {
                    lerEntradaNaoVazia(loan.returnDate, "Data de Devolucao Prevista (DD/MM/AAAA): ");
                    if (!validarData(loan.returnDate)) {
                        cout << "Data inválida. Tente novamente." << endl;
                    } else if (!compararDatas(loan.loanDate, loan.returnDate)) {
                        cout << "Data de devolução não pode ser anterior à data de empréstimo. Tente novamente." << endl;
                    }
                } while (!validarData(loan.returnDate) || !compararDatas(loan.loanDate, loan.returnDate));
                loanList.push_back(loan);
                break;
            }
            case 10: {
                string bookISBN, userId;
                lerEntradaNumerica(bookISBN, "ISBN do Livro: ");
                lerEntradaNumerica(userId, "ID do Usuario: ");
                auto it = remove_if(loanList.begin(), loanList.end(), [&](Loan& loan) {
                    return loan.bookISBN == bookISBN && loan.userId == userId;
                });
                if (it != loanList.end()) {
                    loanList.erase(it, loanList.end());
                    cout << "Devolucao registrada." << endl;
                } else {
                    cout << "Emprestimo nao encontrado." << endl;
                }
                break;
            }
            case 11: {
                for (const Loan& loan : loanList) {
                    cout << "Livro ISBN: " << loan.bookISBN << ", Usuario ID: " << loan.userId << ", Data de Emprestimo: " << loan.loanDate << ", Data de Devolucao Prevista: " << loan.returnDate << endl;
                }
                break;
            }
            case 0: {
                cout << "Saindo..." << endl;
                break;
            }
            default: {
                cout << "Opcao invalida. Tente novamente." << endl;
                break;
            }
        }
    } while (choice != 0);

    return 0;
}
