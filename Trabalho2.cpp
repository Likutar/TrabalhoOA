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
        // cout <<"\nmeh "<< str;
        chaveprim.chave =" ";
        while(linha[i]!=','){
            chaveprim.chave += linha[i];
            i++;
        }
        if(i== linha.length()){
            cout << "erro, arquivo invalido\n";
        }
        raiz->inserir(chaveprim);
        chaveprim.posicao = DB.tellg();
    }
    return raiz;
}
bool Carregar(fstream &arq, BTree *raiz){
        arq.open("indicelista.bt",ios::binary| ios::in);
        if(arq.is_open()){
            int minimo=0;
            arq.read((char*)&minimo,sizeof(int));
            raiz = new BTree(minimo);
            raiz->Carregar(arq);
        }else{
            return false;
        }
        arq.clear();
        arq.close();
        return true;
}

int main(){   
    BTree *raiz= NULL;
    fstream DB, arq;
    string linha;
    DB.open("data.txt");
    if (!Carregar(arq,raiz)){
        cout << "parse\n";
        raiz = parse(DB);
    }
    cout << "arvore pronta\n";
    raiz->Gravar(arq);
    delete raiz;
    return 0;
}