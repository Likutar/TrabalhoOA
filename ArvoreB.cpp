#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;
struct Key{
    string chave;
    long posicao;
    Key(){
        posicao=-1;
        chave = " ";
    }
};
class No{
    Key *chaves; 
    int minimo;  
    No **filhos; 
    int numerodechaves; 
    bool folha; 
public:
    No(int m, bool f){
        minimo = m;
        folha = f;
        chaves = new Key[2*minimo-1];
        filhos = new No *[2*minimo];
        numerodechaves = 0;
    }
    void InserirNaoCheio(Key k){
        int i = numerodechaves-1;
        if (folha == true){
            while (i >= 0 && chaves[i].chave > k.chave){
                chaves[i+1] = chaves[i];
                i--;
            }
            if(i>=0 && chaves[i].chave.compare(k.chave)==0){
                cout << "erro, chave repitida\n";
                
            }else{
                chaves[i+1] = k;
                numerodechaves++;
            }
        }
        else{
            while (i >= 0 && chaves[i].chave > k.chave){
                i--;
            }
            if (filhos[i+1]->numerodechaves == 2*minimo-1){
                Borbulhar(i+1, filhos[i+1]);
                if (chaves[i+1].chave < k.chave){
                    i++;
                }
            }
            filhos[i+1]->InserirNaoCheio(k);
        }
    }
    void Borbulhar(int pos, No *n){
        int i;
        No *novo = new No(n->minimo, n->folha);
        novo->numerodechaves = minimo-1;
    
        for (i=0;i<minimo-1;i++){
            novo->chaves[i] = n->chaves[i+minimo];
        }
        if (n->folha == false){
            for (i=0;i<minimo;i++){
                novo->filhos[i] = n->filhos[i+minimo];
            }
        }
        n->numerodechaves = minimo - 1;
        for (i=numerodechaves;i>=pos+1;i--){
            filhos[i+1] = filhos[i];
        }
        filhos[pos+1] = novo;
        for (i=numerodechaves-1;i>= pos;i--){
            chaves[i+1] = chaves[i];
        }
        chaves[pos] = n->chaves[minimo-1];
        numerodechaves++;
    }
    void percorrer(){
        int i;
        for (i=0;i<numerodechaves; i++){
            if (folha == false){
                filhos[i]->percorrer();
            }
            cout << " " << chaves[i].chave;
        }
        if (folha == false){
            filhos[i]->percorrer();
        }
    }
    void destruir(){
        int i=0;
        for (i=0;i<numerodechaves; i++){
            if (folha == false){
                filhos[i]->destruir();
                
            }
            if(folha ==true){
                delete[] filhos;
                delete[] chaves;
                return;
            }
        }
        if (folha == false){
            filhos[i]->destruir();
        }
    }
    No *busca(Key k){
        int i = 0;
        while (i < numerodechaves && k.chave >chaves[i].chave){
            i++;
        }
        cout << "chave["<< i<<"]= "<<chaves[i].chave<<"k="<<k.chave<<'\n';
        if (chaves[i].chave.compare(k.chave)==0){
            return this;
        }
        if (folha == true){
            return NULL;
        }
        return filhos[i]->busca(k);
    }
    int acharChave(Key k){
        int i=0;
        while (i<numerodechaves && chaves[i].chave < k.chave)
            i++;
        return i;
    }
    long PosicaoRegistro(int i){
        return chaves[i].posicao;
    }
    void remover(Key k){
        bool flag;
        int i = acharChave(k);
        if (i < numerodechaves && chaves[i].chave == k.chave){
            if (folha){
                removerFolha(i);
            }
            else{
                removerNaoFolha(i);
            }
        }
        else{
            if (folha){
                cout << "A chave "<< k.chave <<" nao existe no arquivo\n";
                return;
            }
            if(i==numerodechaves){
                flag=true;
            }else{
                flag=false;
            }
            if (filhos[i]->numerodechaves < minimo){
                redistribuir(i);
            }
            if (flag && i > numerodechaves)
                filhos[i-1]->remover(k);
            else
                filhos[i]->remover(k);
        }
        return;
    }
    void removerFolha (int i){
        int a;
        for (a=i+1; a<numerodechaves; a++){
            chaves[a-1] = chaves[a];
        }
        numerodechaves--;
    }
    void Gravar(fstream& arq){
        int i=0;
        unsigned a=0;
        arq.write((char*)&numerodechaves,sizeof(int));
        arq.write((char*)&folha,sizeof(bool));
        for(i=0;i<numerodechaves;i++){
            a = chaves[i].chave.length();
            arq.write((char*)&a,sizeof(a));
            arq.write(chaves[i].chave.c_str(),sizeof(char)*a);
            arq.write((char*)&chaves[i].posicao,sizeof(long));
        }
        if(folha==false){
            for(i=0;i<=numerodechaves;i++){
                filhos[i]->Gravar(arq);
            }
        }
    }
    void Carregar(fstream& arq){
        int i=0;
        unsigned a=0;
        char *temp= NULL;
        arq.read((char*)&numerodechaves,sizeof(int));
        arq.read((char*)&folha,sizeof(bool));
        for(i=0;i<numerodechaves;i++){
            chaves[i].chave= " ";
            arq.read((char*)&a,sizeof(a));
            temp = new char[a+1];
            arq.read(temp,sizeof(char)*a);
            temp[a] = '\0';
            chaves[i].chave.replace(0,a-1,temp);
            arq.read((char*)&chaves[i].posicao,sizeof(long));
            delete[] temp;
        }
        if(folha==false){
            for(i=0;i<=numerodechaves;i++){
                filhos[i] = new No(minimo, true);
                filhos[i]->Carregar(arq);
            }
        }
    }
    Key Esquerda(int i){
        No *atual=filhos[i];
        while (!atual->folha){
            atual = atual->filhos[atual->numerodechaves];
        }
        return atual->chaves[atual->numerodechaves-1];
    }
    Key Direita(int i){
        No *atual = filhos[i+1];
        while (!atual->folha){
            atual = atual->filhos[0];
        }
        return atual->chaves[0];
    }
    void removerNaoFolha(int i){
        Key k = chaves[i];
        Key esque, dire;
        if (filhos[i]->numerodechaves >= minimo){
            esque = Esquerda(i);
            chaves[i] = esque;
            filhos[i]->remover(esque);
        }
        else if  (filhos[i+1]->numerodechaves >= minimo){
            dire = Direita(i);
            chaves[i] = dire;
            filhos[i+1]->remover(dire);
        }
        else{
            concat(i);
            filhos[i]->remover(k);
        }
    }
    void EsquerdaDireita(int i){
        int a;
        No *filho=filhos[i];
        No *irmao=filhos[i-1];
        for (a=filho->numerodechaves-1; a>=0; a--){
            filho->chaves[a+1] = filho->chaves[a];
        }
        if (!filho->folha){
            for(a=filho->numerodechaves; a>=0;a--){
                filho->filhos[a+1] = filho->filhos[a];
            }
        }
        filho->chaves[0] = chaves[i-1];
        if (!folha){
            filho->filhos[0] = irmao->filhos[irmao->numerodechaves];
        }
        chaves[i-1] = irmao->chaves[irmao->numerodechaves-1];
    
        filho->numerodechaves ++;
        irmao->numerodechaves --;
    }
    void DireitaEsqurda(int i){
        int a;
        No *filho=filhos[i];
        No *irmao=filhos[i+1];
        filho->chaves[(filho->numerodechaves)] = chaves[i];
        if (!(filho->folha)){
            filho->filhos[(filho->numerodechaves)+1] = irmao->filhos[0];
        }
        chaves[i] = irmao->chaves[0];
        for (int a=1; a<irmao->numerodechaves;a++){
            irmao->chaves[a-1] = irmao->chaves[a];
        }
        if (!irmao->folha){
            for(a=1; a<=irmao->numerodechaves;a++){
                irmao->filhos[a-1] = irmao->filhos[a];
            }
        }
        filho->numerodechaves ++;
        irmao->numerodechaves --;
    }
    void concat(int i){
        int a;
        No *filho = filhos[i];
        No *irmao = filhos[i+1];
        filho->chaves[minimo-1] = chaves[i];
        for (int a=0; a<irmao->numerodechaves;a++){
            filho->chaves[a+minimo] = irmao->chaves[a];
        }
        if (!filho->folha){
            for(a=0; a<=irmao->numerodechaves;a++){
                filho->filhos[a+minimo] = irmao->filhos[a];
            }
        }
        for (a=i+1; a<numerodechaves; a++){
            chaves[a-1] = chaves[a];
        }
        for (int a=i+2; a<=numerodechaves; a++){
            filhos[a-1] = filhos[a];
        }
        filho->numerodechaves += irmao->numerodechaves+1;
        numerodechaves--;
        delete(irmao);
    }

    void redistribuir(int i){
        if (i!=0 && filhos[i-1]->numerodechaves>=minimo){
            EsquerdaDireita(i);
        }
        else if (i!=numerodechaves && filhos[i+1]->numerodechaves>=minimo){
            DireitaEsqurda(i);
        }
        else{
            if (i != numerodechaves){
                concat(i);
            }
            else{
                concat(i-1);
            }
        }
    }
 
friend class BTree;
};
 
class BTree{
    No *raiz; 
    int minimo;
    fstream arq;
public:
    BTree(int m){
        raiz = NULL;
        minimo = m;
    }
    ~BTree(){
        raiz->destruir();
    }
    void percorrer(){
        if (raiz != NULL){
            raiz->percorrer();
        }
    }
    No* busca(Key k){  
        if(raiz==NULL){
            return NULL;
        }else{
            return raiz->busca(k);
        }
    }
    void inserir(Key k) {
        if (raiz == NULL){
            raiz = new No(minimo, true);
            raiz->chaves[0] = k;
            raiz->numerodechaves = 1;
        }
        else{
            if (raiz->numerodechaves == 2*minimo-1){
                No *novaraiz = new No(minimo, false);
                novaraiz->filhos[0] = raiz;
                novaraiz->Borbulhar(0, raiz);
                int i = 0;
                if (novaraiz->chaves[0].chave < k.chave){
                    i++;
                }
                novaraiz->filhos[i]->InserirNaoCheio(k);
                raiz = novaraiz;
            }
            else{
                raiz->InserirNaoCheio(k);
            }
        }
    }
    void Gravar(fstream& arq){
            arq.open("indicelista.bt",ios::binary |ios::out);
            if(!arq.is_open()){
                arq.open("indicelista.bt",ios::binary| ios::out|ios::trunc);
            }
            // arq << minimo <<'|';
            arq.write((char*)&minimo,sizeof(int));
            raiz->Gravar(arq);
            arq.close();
    }
    void Carregar(fstream &arq){
        raiz = new No(minimo, true);
        raiz->Carregar(arq);
    }
    void remover(Key k){
        if (!raiz){
            cout << "Arvore vazia\n";
            return;
        }
        raiz->remover(k);
        if (raiz->numerodechaves==0){
            No *temp = raiz;
            if (raiz->folha){
                raiz = NULL;
            }else{
                raiz = raiz->filhos[0];
            }
            delete temp;
        }
    }
    void Escolher_Busca(string &nome){
        int sel;
        fstream DB;
        string linha;
        No *temp;
        Key k;
        long pos;
        bool loop= true;
        while(loop){
            cout <<"\033[2J\033[1;1H";
            cout <<"Qual chave deseja pesquisar?\n";
            getline(cin,k.chave);
            temp = busca(k);
            if (temp ==NULL){
                cout << "erro, chave nao encontrada, deseja procurar outra?\n1)Sim\n2)Nao\n";
                getline(cin,linha);
                sel = atoi(linha.c_str());
                if(sel==2){
                    loop=false;
                }
            }else{
                pos = temp->PosicaoRegistro(temp->acharChave(k));
                DB.open(nome.c_str());
                if(DB.is_open()){
                    DB.seekg(pos);
                    getline(DB,linha);
                    cout << linha << '\n';
                    cout << "deseja procurar outra chave?\n1)Sim\n2)Nao\n";
                    getline(cin,linha);
                    sel = atoi(linha.c_str());
                    if(sel==2){
                        loop=false;
                    }
                }else{
                    cout << "erro, arquivo nao encontrado ";
                    loop=false;
                }
                DB.close();
            }
        }
    }
    void Excluir_Registro(string &nome, string &nomeindice){
        int sel;
        fstream DB;
        string linha;
        No *temp;
        Key k;
        long pos;
        bool loop= true;
        while(loop){
            cout <<"\033[2J\033[1;1H";
            cout <<"Qual chave deseja excluir?\n";
            getline(cin,k.chave);
            temp = busca(k);
            if (temp ==NULL){
                cout << "erro, chave nao encontrada, deseja procurar outra?\n1)Sim\n2)Nao\n";
                getline(cin,linha);
                sel = atoi(linha.c_str());
                if(sel==2){
                    loop=false;
                }
            }else{
                pos = temp->PosicaoRegistro(temp->acharChave(k));
                DB.open(nome.c_str());
                if(DB.is_open()){
                    DB.seekg(pos);
                    getline(DB,linha);
                    DB.seekg(pos);
                    DB.seekp(pos);
                    linha.replace(0,string::npos,linha.length(),' ');
                    DB<< linha;
                    remover(k);
                    cout << "deseja excluir outra chave?\n1)Sim\n2)Nao\n";
                    getline(cin,linha);
                    sel = atoi(linha.c_str());
                    if(sel==2){
                        loop=false;
                    }
                }else{
                    cout << "erro, arquivo nao encontrado ";
                    loop=false;
                }
                DB.close();
            }
        }
    }
    void Inserir_Registro(string &nome, string &nomeindice){
        int sel;
        fstream DB;
        string linha,str;
        No *temp;
        Key k;
        long pos;
        bool loop= true;
        while(loop){
            cout <<"\033[2J\033[1;1H";
            cout <<"Forneça a chave primaria:\n";
            getline(cin,k.chave);
            temp = busca(k);
            if(temp ==NULL){
                str+='\n';
                str+= k.chave;
                for(sel=0;sel<18;sel++){
                    cout << "insira o valor do campo numero "<< sel+1<<":\n";
                    getline(cin,linha);
                    str+= ',';
                    str+= linha;
                }
                DB.open(nome.c_str(), ios::app);
                if(DB.is_open()){
                    DB.clear();
                    DB << str;
                    k.posicao = DB.tellp();
                    inserir(k);
                    DB.close();
                    cout << "deseja adicionar outra chave?\n1)Sim\n2)Nao\n";
                    getline(cin,linha);
                    sel = atoi(linha.c_str());
                    if(sel==2){
                        loop=false;
                    }
                }else{
                    cout << "erro, arquivo nao encontrado ";
                    loop=false;
                }
            }else{
                cout << "essa chave primaria ja existe, tente outra\n";
            }
        }
    }
    void Alterar_Registro(string &nome, string &nomeindice){
        int sel, i, a, b;
        fstream DB;
        string linha,str, novo;
        No *temp;
        Key k;
        long pos;
        cout <<"\033[2J\033[1;1H";
        cout <<"Forneça a chave primaria:\n";
        getline(cin,k.chave);
        temp = busca(k);
        if(temp !=NULL){
            DB.open(nome.c_str());
            if(DB.is_open()){
                pos = temp->PosicaoRegistro(temp->acharChave(k));
                DB.seekg(pos);
                getline(DB,str);
                cout << str << '\n';
                cout << "qual camp deseja editar?[1-18]\n";
                getline(cin,linha);
                sel = atoi(linha.c_str());
                i=0;
                a=0;
                b=0;
                while(i<sel-1){
                    a++;
                    if(str[a]==','){
                        i++;
                    }
                }
                a++;
                b=a;
                while(i!=sel){
                    b++;
                    if(str[b]==','){
                        i++;
                    }
                    
                }
                linha = '\n';
                if(sel ==1){
                    cout << "qual o novo valor?\n";
                    getline(cin,novo);
                    linha += novo;
                    linha+= str.substr(b,string::npos);
                }else{
                    linha += str.substr(0,a);
                    cout << "qual o novo valor?\n";
                    getline(cin,novo);
                    linha += novo;
                    linha+= str.substr(b,string::npos);
                }
                str.replace(0,string::npos,str.length(),' ');
                DB.seekp(pos);
                DB << str;
                DB.seekp(0,ios::end);
                pos = DB.tellp();
                DB << linha;
                DB.clear();
                pos++;
                // cout << "pos ="<< pos<<"\nlinha ="<< linha;
                if(sel==1){
                    remover(k);
                    k.chave = novo;
                    k.posicao = pos;
                    inserir(k);
                }else{
                    temp->chaves[temp->acharChave(k)].posicao = pos;
                }
                DB.clear();
                DB.close();
            }else{
                cout << "erro, arquivo nao encontrado ";
            }
            DB.close();
        }else{
            cout << "essa chave primaria nao existe\n";
        }
    }
};
