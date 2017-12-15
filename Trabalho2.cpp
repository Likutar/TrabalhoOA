#include<cmath>
#include <fstream>
#include"ArvoreB.cpp"

int CalcularMinimo(double tamanho){
    double m=3.0;
    double n,temp,i;
    n=0.0;
    i=0.0;
    n = tamanho/2.0;
    n = log(n);
    temp = 0.0;
    temp=ceil(m/2.0);
    i=temp;
    temp=log(temp);
    while((1+n/temp)>=4.0){
        m++;
        temp=ceil(m/2);
        i=temp;
        temp=log(temp);
    }
    return (int) i;
}
BTree* parse(fstream &DB){
    unsigned i, tamanho;
    string linha;
    Key chaveprim;
    BTree *raiz;
    tamanho=0;
    DB.clear();
    DB.seekg(ios::beg);
    while(getline(DB,linha)){
         tamanho++;
    }
    DB.clear();
    DB.seekg(ios::beg);
    i = CalcularMinimo(tamanho);
    raiz = new BTree(i);
    chaveprim.posicao = DB.tellg();
    while(getline(DB,linha)){
        i=0;
        chaveprim.chave =" ";
        while(linha[i]!=','){
            i++;
        }
        chaveprim.chave = linha.substr(0, i);
        if(i== linha.length()){
            cout << "erro, arquivo invalido\n";
        }
        raiz->inserir(chaveprim);
        chaveprim.posicao = DB.tellg();
        linha =" ";
    }
    return raiz;
}
BTree* Carregar(string &nomeindice){
    int minimo;
    fstream arq;
    BTree *raiz=NULL;
    arq.open((nomeindice).c_str(),ios::binary| ios::in);
    if(arq.is_open()){
        arq.read((char*)&minimo,sizeof(int));
        raiz = new BTree(minimo);
        raiz->Carregar(arq);
    }else{
        return NULL;
    }
    arq.clear();
    arq.close();
    return raiz;
}

int main(){   
    BTree *raiz= NULL;
    fstream DB, arq;
    string linha, nome,nomeindice;
    int sel=0;
    bool loop=true;
    cout <<"\033[2J\033[1;1H";
    cout <<"Qual arquivo deseja abrir?\n";
    cout <<"1) data.txt\n2) outro\n";
    getline(cin,linha);
    if (linha[0]== '2'){
        cout<< "Qual o nome do arquivo?\n";
        cin >> nome;
        sel=0;
        while(nome[sel]!='.'){
            nomeindice+= nome[sel];
            sel++;
        }
        nomeindice+="_indicelista.bt";
        raiz = Carregar(nomeindice);
        if (raiz == NULL){
            DB.open(nome.c_str());
            raiz = parse(DB);
            DB.close();
        }
    }
    else{
        nome = "data.txt";
        nomeindice ="indicelista.bt";
        raiz = Carregar(nomeindice);
        cout << "ok\n";
        if (raiz == NULL){
            DB.open("data.txt");
            raiz = parse(DB);
            DB.close();
        }   
    }
    while (loop){
        cout <<"\033[2J\033[1;1H";
        cout <<"Qual operação deseja realizar\n1) Buscar\n2) adicionar\n3)excluir\n4)alterar\n5)sair\n";
        getline(cin,linha);
        sel=atoi(linha.c_str());
        switch(sel){
            case 1:
                raiz->Escolher_Busca(nome);
                break;
            case 2:
                raiz->Inserir_Registro(nome, nomeindice);
                break;
            case 3:
                raiz->Excluir_Registro(nome, nomeindice);
                break;
            case 4:
                raiz->Alterar_Registro(nome, nomeindice);
                break;
            default:
                loop = 0;
        }
    }
    cout << "arvore pronta\n";
    raiz->Gravar(arq);
    delete raiz;
    return 0;
}