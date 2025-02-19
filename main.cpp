#include <iostream>
#include <fstream>
#include <sstream>
#include <limits> // Para limpar o buffer do cin
using namespace std;

// Estrutura para representar os carros
struct Carro {
    int id;
    string modelo;
    int anoFabricacao;
    string nomeMarca;
    float altura;
    float peso;
    string cor;
    string paisOrigemMarca;
    string tipoCombustivel;
    string cambio;
    float torque;
    int velocidadeMaxima;
    bool ativo;
};
// Função para carregar dados do arquivo CSV
Carro* carregarDadosCSV(const string& nomeArquivo, int& quantidade, int& capacidade) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo CSV: " << nomeArquivo << endl;
        return nullptr;
    }

    string linha;
    Carro* carros = new Carro[capacidade];
    quantidade = 0;

    // Ignora o cabeçalho
    getline(arquivo, linha);

    while (getline(arquivo, linha)) {
        if (quantidade >= capacidade) {
            capacidade *= 2;
            Carro* novoCarros = new Carro[capacidade];
            for (int i = 0; i < quantidade; ++i) novoCarros[i] = carros[i];
            delete[] carros;
            carros = novoCarros;
        }

        stringstream ss(linha);
        string campo;

        try {
            // Lê o id
            getline(ss, campo, ',');
            carros[quantidade].id = campo.empty() ? 0 : stoi(campo);

            // Lê os demais campos
            getline(ss, campo, ',');
            carros[quantidade].modelo = campo;

            getline(ss, campo, ',');
            carros[quantidade].anoFabricacao = campo.empty() ? 0 : stoi(campo);

            getline(ss, campo, ',');
            carros[quantidade].nomeMarca = campo;

            getline(ss, campo, ',');
            carros[quantidade].altura = campo.empty() ? 0.0f : stof(campo);

            getline(ss, campo, ',');
            carros[quantidade].peso = campo.empty() ? 0.0f : stof(campo);

            getline(ss, campo, ',');
            carros[quantidade].cor = campo;

            getline(ss, campo, ',');
            carros[quantidade].paisOrigemMarca = campo;

            getline(ss, campo, ',');
            carros[quantidade].tipoCombustivel = campo;

            getline(ss, campo, ',');
            carros[quantidade].cambio = campo;

            getline(ss, campo, ',');
            carros[quantidade].torque = campo.empty() ? 0.0f : stof(campo);

            getline(ss, campo, ',');
            carros[quantidade].velocidadeMaxima = campo.empty() ? 0 : stoi(campo);

            carros[quantidade].ativo = true; // Por padrão, o carro está ativo
            quantidade++;
        } catch (...) {
            cerr << "Erro ao processar linha: " << linha << endl;
           
        }
    }

    arquivo.close();
    return carros;
}

// Função para salvar os dados no arquivo binário
void salvarDadosBinario(const string& nomeArquivo, const Carro* carros, int quantidade) {
    ofstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo binário para escrita: " << nomeArquivo << endl;
        return;
    }

    // Escreve a quantidade de carros no arquivo
    arquivo.write(reinterpret_cast<const char*>(&quantidade), sizeof(quantidade));

    // Escreve cada carro no arquivo
    for (int i = 0; i < quantidade; ++i) {
        arquivo.write(reinterpret_cast<const char*>(&carros[i]), sizeof(Carro));
    }

    arquivo.close();
    cout << "Dados salvos no arquivo binário com sucesso!" << endl;
}

// Função de comparação para ordenação
bool compararCarros(const Carro& a, const Carro& b) {
    if (a.modelo == b.modelo) {
        return a.anoFabricacao < b.anoFabricacao;
    }
    return a.modelo < b.modelo;
}


// Função para particionar o vetor (usada no Quicksort)
int particionar(Carro* carros, int inicio, int fim) {
    Carro pivo = carros[fim]; // Escolhe o último elemento como pivô
    int i = inicio - 1; // Índice do menor elemento

    for (int j = inicio; j < fim; ++j) {
        if (compararCarros(carros[j], pivo)) {
            i++;
            swap(carros[i], carros[j]); // Troca os elementos
        }
    }
    swap(carros[i + 1], carros[fim]); // Coloca o pivô na posição correta
    return i + 1; // Retorna o índice do pivô
}



// Função Quicksort
void quicksort(Carro* carros, int inicio, int fim) {
    if (inicio < fim) {
        int indicePivo = particionar(carros, inicio, fim); // Particiona o vetor
        quicksort(carros, inicio, indicePivo - 1); // Ordena a partição esquerda
        quicksort(carros, indicePivo + 1, fim); // Ordena a partição direita
    }
}
// Função para ordenar o vetor de carros
void ordenarCarros(Carro* carros, int quantidade) {
    quicksort(carros, 0, quantidade - 1);
}

// Função para exibir os carros com paginação (ignora registros inativos)
void exibirCarros(Carro* carros, int quantidade) {
    if (quantidade == 0) {
        cout << "Nenhum carro carregado." << endl;
        return;
    }

    int carrosPorPagina = 20;
    int totalPaginas = (quantidade + carrosPorPagina - 1) / carrosPorPagina;
    int paginaAtual = 1;

    while (true) {
        cout << "===== Página " << paginaAtual << " de " << totalPaginas << " =====" << endl;
        int inicio = (paginaAtual - 1) * carrosPorPagina;
        int fim = min(inicio + carrosPorPagina, quantidade);

        int carrosExibidos = 0;
        for (int i = inicio; i < fim; ++i) {
            if (carros[i].ativo) { // Exibe apenas carros ativos
                cout << "ID: " << carros[i].id << endl;
                cout << "Modelo: " << carros[i].modelo << endl;
                cout << "Ano de Fabricação: " << carros[i].anoFabricacao << endl;
                cout << "Marca: " << carros[i].nomeMarca << endl;
                cout << "Altura: " << carros[i].altura << " m" << endl;
                cout << "Peso: " << carros[i].peso << " kg" << endl;
                cout << "Cor: " << carros[i].cor << endl;
                cout << "País de Origem: " << carros[i].paisOrigemMarca << endl;
                cout << "Tipo de Combustível: " << carros[i].tipoCombustivel << endl;
                cout << "Câmbio: " << carros[i].cambio << endl;
                cout << "Torque: " << carros[i].torque << " Nm" << endl;
                cout << "Velocidade Máxima: " << carros[i].velocidadeMaxima << " km/h" << endl;
                cout << "-----------------------------" << endl;
                carrosExibidos++;
            }
        }

        if (carrosExibidos == 0) {
            cout << "Nenhum carro ativo nesta página." << endl;
        }

        cout << "Opções: (P) Próxima página, (A) Página anterior, (O) Ordem Alfabética, (S) Sair" << endl;
        char opcao;
        cin >> opcao;

        if (opcao == 'P' || opcao == 'p') {
            if (paginaAtual < totalPaginas) {
                paginaAtual++;
            } else {
                cout << "Você já está na última página." << endl;
            }
        } else if (opcao == 'A' || opcao == 'a') {
            if (paginaAtual > 1) {
                paginaAtual--;
            } else {
                cout << "Você já está na primeira página." << endl;
            }
        } else if (opcao == 'O' || opcao == 'o') {
            // Ordena os carros por modelo e ano de fabricação
            ordenarCarros(carros, quantidade);
            cout << "Carros ordenados por modelo e ano de fabricação!" << endl;
        } else if (opcao == 'S' || opcao == 's') {
            break;
        } else {
            cout << "Opção inválida! Tente novamente." << endl;
        }
    }
}
// Função para inserir um novo carro no vetor dinâmico
void inserirCarro(Carro*& carros, int& quantidade, int& capacidade, const string& nomeArquivoBinario) {
    // Verifica se o vetor precisa ser redimensionado
    if (quantidade >= capacidade) {
        capacidade *= 2; // Dobra a capacidade
        Carro* novoCarros = new Carro[capacidade]; // Aloca um novo vetor com a nova capacidade

        // Copia os carros existentes para o novo vetor
        for (int i = 0; i < quantidade; ++i) {
            novoCarros[i] = carros[i];
        }

        // Libera a memória do vetor antigo
        delete[] carros;

        // Atualiza o ponteiro para o novo vetor
        carros = novoCarros;
    }

    Carro novoCarro;

    // Função para validar se uma string está vazia
    auto validarString = [](const string& campo) {
        string valor;
        while (true) {
            getline(cin, valor);
            if (!valor.empty()) {
                return valor;
            }
            cout << "O campo " << campo << " não pode ser vazio. Por favor, insira novamente: ";
        }
    };

    // Função para validar se um número é válido (não negativo)
    auto validarNumero = [](const string& campo) {
        int valor;
        while (true) {
            cin >> valor;
            if (cin.fail() || valor < 0) {
                cin.clear(); // Limpa o estado de erro do cin
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpa o buffer
                cout << "O campo " << campo << " não pode ser negativo ou inválido. Por favor, insira novamente: ";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpa o buffer
                return valor;
            }
        }
    };

    // Função para validar se um número float é válido (não negativo)
    auto validarFloat = [](const string& campo) {
        float valor;
        while (true) {
            cin >> valor;
            if (cin.fail() || valor < 0) {
                cin.clear(); // Limpa o estado de erro do cin
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpa o buffer
                cout << "O campo " << campo << " não pode ser negativo ou inválido. Por favor, insira novamente: ";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpa o buffer
                return valor;
            }
        }
    };

    cout << "Modelo: ";
    novoCarro.modelo = validarString("Modelo");

    cout << "Ano de Fabricação: ";
    novoCarro.anoFabricacao = validarNumero("Ano de Fabricação");

    cout << "Marca: ";
    novoCarro.nomeMarca = validarString("Marca");

    cout << "Altura: ";
    novoCarro.altura = validarFloat("Altura");

    cout << "Peso: ";
    novoCarro.peso = validarFloat("Peso");

    cout << "Cor: ";
    novoCarro.cor = validarString("Cor");

    cout << "País de Origem: ";
    novoCarro.paisOrigemMarca = validarString("País de Origem");

    cout << "Tipo de Combustível: ";
    novoCarro.tipoCombustivel = validarString("Tipo de Combustível");

    cout << "Câmbio: ";
    novoCarro.cambio = validarString("Câmbio");

    cout << "Torque: ";
    novoCarro.torque = validarFloat("Torque");

    cout << "Velocidade Máxima: ";
    novoCarro.velocidadeMaxima = validarNumero("Velocidade Máxima");

    // Atribui um ID sequencial ao novo carro
    novoCarro.id = quantidade + 1;
    novoCarro.ativo = true; // Novo carro é marcado como ativo

    // Adiciona o novo carro ao vetor
    carros[quantidade] = novoCarro;
    quantidade++;

    cout << "Carro inserido com sucesso! ID: " << novoCarro.id << endl;

    // Salva os dados no arquivo binário após a inserção
    salvarDadosBinario(nomeArquivoBinario, carros, quantidade);
}






// Função para remover um carro logicamente (marca como inativo) com base no id
void removerCarro(Carro* carros, int quantidade, const string& nomeArquivoBinario) {
    if (quantidade == 0) {
        cout << "Nenhum carro carregado." << endl;
        return;
    }

    cout << "Digite o ID do carro que deseja remover: ";
    int id;
    cin >> id;

    bool encontrado = false;
    for (int i = 0; i < quantidade; ++i) {
        if (carros[i].id == id && carros[i].ativo) {
            carros[i].ativo = false; // Marca o carro como inativo
            cout << "Carro com ID " << id << " removido com sucesso!" << endl;
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        cout << "Carro com ID " << id << " não encontrado ou já foi removido." << endl;
    }

    // Salva os dados no arquivo binário após a remoção
    salvarDadosBinario(nomeArquivoBinario, carros, quantidade);
}








// Função de busca binária para modelo e ano de fabricação
int buscaBinaria(const Carro* carros, int quantidade, const string& modelo, int anoFabricacao) {
    int esquerda = 0;
    int direita = quantidade - 1;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;

        if (carros[meio].modelo == modelo && carros[meio].anoFabricacao == anoFabricacao) {
            return meio; // Retorna o índice do carro encontrado
        }

        if (carros[meio].modelo < modelo || (carros[meio].modelo == modelo && carros[meio].anoFabricacao < anoFabricacao)) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }

    return -1; // Retorna -1 se o carro não for encontrado
}

// Função para buscar um carro pelo modelo e ano de fabricação
void buscarCarro(const Carro* carros, int quantidade) {
    if (quantidade == 0) {
        cout << "Nenhum carro carregado." << endl;
        return;
    }

    string modelo;
    int anoFabricacao;

    cout << "Digite o modelo do carro: ";
    cin >> modelo;
    cout << "Digite o ano de fabricação do carro: ";
    cin >> anoFabricacao;

    int indice = buscaBinaria(carros, quantidade, modelo, anoFabricacao);

    if (indice != -1 && carros[indice].ativo) {
        cout << "Carro encontrado:" << endl;
        cout << "-----------------------------" << endl;
        cout << "ID: " << carros[indice].id << endl;
        cout << "Modelo: " << carros[indice].modelo << endl;
        cout << "Ano de Fabricação: " << carros[indice].anoFabricacao << endl;
        cout << "Marca: " << carros[indice].nomeMarca << endl;
        cout << "Altura: " << carros[indice].altura << " m" << endl;
        cout << "Peso: " << carros[indice].peso << " kg" << endl;
        cout << "Cor: " << carros[indice].cor << endl;
        cout << "País de Origem: " << carros[indice].paisOrigemMarca << endl;
        cout << "Tipo de Combustível: " << carros[indice].tipoCombustivel << endl;
        cout << "Câmbio: " << carros[indice].cambio << endl;
        cout << "Torque: " << carros[indice].torque << " Nm" << endl;
        cout << "Velocidade Máxima: " << carros[indice].velocidadeMaxima << " km/h" << endl;
        cout << "-----------------------------" << endl;
    } else {
        cout << "Carro não encontrado ou foi removido." << endl;
    }
}

// Função para exibir o arquivo completo ou um trecho específico
void exibirArquivo(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return;
    }

    cout << "Escolha uma opção:" << endl;
    cout << "1. Exibir o arquivo completo" << endl;
    cout << "2. Exibir um trecho específico" << endl;
    int opcao;
    cin >> opcao;

    if (opcao == 1) {
        string linha;
        while (getline(arquivo, linha)) {
            cout << linha << endl;
        }
    } else if (opcao == 2) {
        int inicio, fim;
        cout << "Digite a linha de início: ";
        cin >> inicio;
        cout << "Digite a linha de fim: ";
        cin >> fim;

        string linha;
        int contador = 1;
        while (getline(arquivo, linha)) {
            if (contador >= inicio && contador <= fim) {
                cout << linha << endl;
            }
            if (contador > fim) {
                break;
            }
            contador++;
        }
    } else {
        cout << "Opção inválida!" << endl;
    }

    arquivo.close();
}

// Função para exportar os dados dos carros para um arquivo CSV
void exportarParaCSV(const string& nomeArquivo, const Carro* carros, int quantidade) {
    ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo CSV para escrita: " << nomeArquivo << endl;
        return;
    }

    // Escreve o cabeçalho do CSV
    arquivo << "id,Modelo,AnoFabricacao,Marca,Altura,Peso,Cor,PaisOrigem,Combustivel,Cambio,Torque,VelocidadeMaxima,Ativo\n";

    // Escreve os dados dos carros
    for (int i = 0; i < quantidade; ++i) {
        if (carros[i].ativo) { // Exporta apenas carros ativos
            arquivo << carros[i].id << ","
                    << carros[i].modelo << ","
                    << carros[i].anoFabricacao << ","
                    << carros[i].nomeMarca << ","
                    << carros[i].altura << ","
                    << carros[i].peso << ","
                    << carros[i].cor << ","
                    << carros[i].paisOrigemMarca << ","
                    << carros[i].tipoCombustivel << ","
                    << carros[i].cambio << ","
                    << carros[i].torque << ","
                    << carros[i].velocidadeMaxima << ","
                    << (carros[i].ativo ? "true" : "false") << "\n";
        }
    }

    arquivo.close();
    cout << "Dados exportados para o arquivo CSV com sucesso: " << nomeArquivo << endl;
}

// Função para importar dados de um arquivo CSV
void importarDeCSV(Carro*& carros, int& quantidade, int& capacidade, const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo CSV para leitura: " << nomeArquivo << endl;
        return;
    }

    string linha;
    getline(arquivo, linha); // Ignora o cabeçalho

    while (getline(arquivo, linha)) {
        if (quantidade >= capacidade) {
            capacidade *= 2;
            Carro* novoCarros = new Carro[capacidade];
            for (int i = 0; i < quantidade; ++i) novoCarros[i] = carros[i];
            delete[] carros;
            carros = novoCarros;
        }

        stringstream ss(linha);
        string campo;

        try {
            getline(ss, campo, ',');
            carros[quantidade].id = campo.empty() ? 0 : stoi(campo);

            getline(ss, campo, ',');
            carros[quantidade].modelo = campo;

            getline(ss, campo, ',');
            carros[quantidade].anoFabricacao = campo.empty() ? 0 : stoi(campo);

            getline(ss, campo, ',');
            carros[quantidade].nomeMarca = campo;

            getline(ss, campo, ',');
            carros[quantidade].altura = campo.empty() ? 0.0f : stof(campo);

            getline(ss, campo, ',');
            carros[quantidade].peso = campo.empty() ? 0.0f : stof(campo);

            getline(ss, campo, ',');
            carros[quantidade].cor = campo;

            getline(ss, campo, ',');
            carros[quantidade].paisOrigemMarca = campo;

            getline(ss, campo, ',');
            carros[quantidade].tipoCombustivel = campo;

            getline(ss, campo, ',');
            carros[quantidade].cambio = campo;

            getline(ss, campo, ',');
            carros[quantidade].torque = campo.empty() ? 0.0f : stof(campo);

            getline(ss, campo, ',');
            carros[quantidade].velocidadeMaxima = campo.empty() ? 0 : stoi(campo);

            getline(ss, campo, ',');
            carros[quantidade].ativo = (campo == "true");

            quantidade++;
        } catch (...) {
            cerr << "Erro ao processar linha: " << linha << endl;
            continue;
        }
    }

    arquivo.close();
    cout << "Dados importados do arquivo CSV com sucesso: " << nomeArquivo << endl;
}

// Função para exibir o menu interativo
void exibirMenuInterativo() {
    cout << "=====================================" << endl;
    cout << "          MENU DE NAVEGAÇÃO          " << endl;
    cout << "=====================================" << endl;
    cout << "1. Carregar dados do arquivo CSV" << endl;
    cout << "2. Exibir carros carregados" << endl;
    cout << "3. Inserir novo carro" << endl;
    cout << "4. Remover carro" << endl;
    cout << "5. Buscar carro por modelo e ano de fabricação" << endl;
    cout << "6. Exibir arquivo completo ou trecho específico" << endl;
    cout << "7. Exportar dados para CSV" << endl;
    cout << "8. Importar dados de CSV" << endl;
    cout << "9. Sair" << endl;
    cout << "====================================" << endl;
    cout << "Escolha uma opção: ";
}

int main() {
    int quantidade = 0;
    int capacidade = 10;
    Carro* carros = nullptr;
    string nomeArquivoCSV = "carros_dados_completos.csv"; // Nome do arquivo CSV padrão
    string nomeArquivoBinario = "carros_dados.bin"; // Nome do arquivo binário

    int opcao;
    do {
        exibirMenuInterativo();
        cin >> opcao;

        switch (opcao) {
            case 1:
                if (carros) {
                    delete[] carros; // Libera memória se já houver carros carregados
                }
                carros = carregarDadosCSV(nomeArquivoCSV, quantidade, capacidade);
                if (carros) {
                    cout << "Dados carregados com sucesso! Total de carros: " << quantidade << endl;
                } else {
                    cout << "Falha ao carregar os dados do arquivo CSV." << endl;
                }
                break;

            case 2:
                exibirCarros(carros, quantidade);
                break;

            case 3:
                inserirCarro(carros, quantidade, capacidade, nomeArquivoBinario);
                break;

            case 4:
                removerCarro(carros, quantidade, nomeArquivoBinario);
                break;

            case 5:
                buscarCarro(carros, quantidade);
                break;

            case 6:
                exibirArquivo(nomeArquivoCSV);
                break;

            case 7: {
                string nomeArquivoExportacao;
                cout << "Digite o nome do arquivo CSV para exportação: ";
                cin >> nomeArquivoExportacao;
                exportarParaCSV(nomeArquivoExportacao, carros, quantidade);
                break;
            }

            case 8: {
                string nomeArquivoImportacao;
                cout << "Digite o nome do arquivo CSV para importação: ";
                cin >> nomeArquivoImportacao;
                importarDeCSV(carros, quantidade, capacidade, nomeArquivoImportacao);
                break;
            }

            case 9:
                cout << "Saindo..." << endl;
                break;

            default:
                cout << "Opção inválida! Tente novamente." << endl;
                break;
        }
    } while (opcao != 9);

    if (carros) {
        delete[] carros; // Libera a memória alocada para os carros
    }

    return 0;
}