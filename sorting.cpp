#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cmath>
#include <iomanip>
using namespace std;

long long insertionSort(vector<int>& arr) {
    long long moves = 0;
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
            moves++;
        }
        arr[j + 1] = key;
        if (j + 1 != i) moves++; 
    }
    return moves;
}
long long mergeCount = 0;

void merge(vector<int>& arr, int esq, int meio, int dir) {
    vector<int> temp(dir - esq + 1);
    int i = esq, j = meio + 1, k = 0;

    while (i <= meio && j <= dir) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else                  temp[k++] = arr[j++];
        mergeCount++;
    }
    while (i <= meio) { temp[k++] = arr[i++]; mergeCount++; }
    while (j <= dir)  { temp[k++] = arr[j++]; mergeCount++; }

    for (int x = 0; x < k; x++) arr[esq + x] = temp[x];
}

void mergeSortRec(vector<int>& arr, int esq, int dir) {
    if (esq >= dir) return;
    int meio = esq + (dir - esq) / 2;
    mergeSortRec(arr, esq, meio);
    mergeSortRec(arr, meio + 1, dir);
    merge(arr, esq, meio, dir);
}

long long mergeSort(vector<int>& arr) {
    mergeCount = 0;
    mergeSortRec(arr, 0, arr.size() - 1);
    return mergeCount;
}
long long heapCount = 0;

void heapify(vector<int>& arr, int n, int i) {
    int maior = i;
    int esq   = 2 * i + 1;
    int dir   = 2 * i + 2;

    if (esq < n && arr[esq] > arr[maior]) maior = esq;
    if (dir < n && arr[dir] > arr[maior]) maior = dir;

    if (maior != i) {
        swap(arr[i], arr[maior]);
        heapCount++;
        heapify(arr, n, maior);
    }
}

long long heapSort(vector<int>& arr) {
    heapCount = 0;
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapCount++;
        heapify(arr, i, 0);
    }
    return heapCount;
}

vector<int> gerarVetor(int n, int seed) {
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(1, 1000000);
    vector<int> v(n);
    for (int i = 0; i < n; i++) v[i] = dist(rng);
    return v;
}

struct Resultado {
    double t1, t2, t3, media, desvio;
    long long trocas;
};

template<typename Func>
Resultado executar3x(Func sortFunc, const vector<int>& original) {
    Resultado r;
    double tempos[3];

    for (int exec = 0; exec < 3; exec++) {
        vector<int> arr = original; 
        auto ini = chrono::high_resolution_clock::now();
        long long trocas = sortFunc(arr);
        auto fim = chrono::high_resolution_clock::now();
        tempos[exec] = chrono::duration<double>(fim - ini).count();
        if (exec == 2) r.trocas = trocas;
    }

    r.t1 = tempos[0];
    r.t2 = tempos[1];
    r.t3 = tempos[2];
    r.media = (r.t1 + r.t2 + r.t3) / 3.0;
    r.desvio = sqrt(((r.t1-r.media)*(r.t1-r.media) +
                     (r.t2-r.media)*(r.t2-r.media) +
                     (r.t3-r.media)*(r.t3-r.media)) / 3.0);
    return r;
}

void imprimirResultado(const string& algo, int n, const Resultado& r) {
    cout << fixed << setprecision(6);
    cout << algo << "\t" << n << "\t"
         << r.t1    << "\t"
         << r.t2    << "\t"
         << r.t3    << "\t"
         << r.media << "\t"
         << r.desvio << "\t"
         << r.trocas << endl;
}

int main() {
    vector<int> tamanhos = {1000, 10000, 100000};
    int SEED = 42;

    cout << "Algoritmo\tTamanho\tExec1(s)\tExec2(s)\tExec3(s)\tMedia(s)\tDesvio(s)\tTrocas" << endl;
    cout << string(100, '-') << endl;

    for (int n : tamanhos) {
        vector<int> original = gerarVetor(n, SEED);

        // Insertion Sort
        {
            auto r = executar3x(insertionSort, original);
            imprimirResultado("Insertion Sort", n, r);
        }

        // Merge Sort
        {
            auto r = executar3x(mergeSort, original);
            imprimirResultado("Merge Sort", n, r);
        }

        // Heap Sort
        {
            auto r = executar3x(heapSort, original);
            imprimirResultado("Heap Sort", n, r);
        }

        cout << endl;
    }

    return 0;
}
