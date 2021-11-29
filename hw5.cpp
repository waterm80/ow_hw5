/*
Student No.: 0816058
Student Name: 林鴻廷
Email: waterm80.cs08@nycu.edu.tw
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not supposed
to be posted to be a public server, such as a public GitHub
repository or a public web page.
*/

#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <list>
#include <utility>
#include <vector>
#include <iomanip>
#include <climits>
using namespace std;

int hit, miss, seq, f, heap_size;
pair<int, pair<int, int>> *harr; // key, <count, seq>
pair<int, int> *harr2;  // key, seq
list<int> *table;    // key


void swap(pair<int, pair<int, int>> *x, pair<int, pair<int, int>> *y){
    pair<int, pair<int, int>> temp = *x;
    *x = *y;
    *y = temp;
}

int parent(int i){
    return (i-1)/2; 
}

void insertKey(pair<int, pair<int, int>> k){
    heap_size++;
    int i = heap_size - 1;
    harr[i] = k;
    while (i != 0 && harr[parent(i)].second > harr[i].second){
       swap(&harr[i], &harr[parent(i)]);
       i = parent(i);
    }
}

void MinHeapify(int i){
    int l = 2*i + 1;
    int r = 2*i + 2;
    int smallest = i;
    if(l < heap_size && harr[l].second < harr[i].second)
        smallest = l;
    if(r < heap_size && harr[r].second < harr[smallest].second)
        smallest = r;
    if(smallest != i){
        swap(&harr[i], &harr[smallest]);
        MinHeapify(smallest);
    }
}

pair<int, pair<int, int>> extractMin(){
    if (heap_size == 1){
        heap_size--;
        return harr[0];
    }
    pair<int, pair<int, int>> root = harr[0];
    harr[0] = harr[heap_size-1];
    heap_size--;
    MinHeapify(0);
    return root;
}

void lfu(int key){
    int index = key % f;
    list<int>::iterator i;
    for(i = table[index].begin(); i != table[index].end(); i++)
        if(*i == key)
            break;
    if(i != table[index].end()){
        hit++;
        for(int j = 0; j < heap_size; j++)
            if(harr[j].first == key){
                harr[j].second.first++;
                harr[j].second.second = seq++;
                MinHeapify(j);
                break;
            }
    }
    else{
        miss++;
        if(heap_size == f){
            pair<int, pair<int, int>> p = harr[0];
            extractMin();
            list<int>::iterator it;
            int dindex = p.first % f;
            for(it = table[dindex].begin(); it != table[dindex].end(); it++)
                if(*it == p.first){
                    table[dindex].erase(it);
                    break;
                }
            table[index].push_back(key);
            insertKey(make_pair(key, make_pair(1, seq++)));
        }
        else{
            table[index].push_back(key);
            insertKey(make_pair(key, make_pair(1, seq++)));
        }
    }
}

void LFU(char* file){
    struct timeval start, end;
    gettimeofday(&start, 0);
    cout << "LFU policy:\nFrame\tHit\t\tMiss\t\tPage fault ratio\n";
    for(f = 64; f <= 512; f *= 2){
        string page;
        fstream ifs;
        table = new list<int>[f];
        harr = new pair<int, pair<int, int>>[f];
        hit = miss = seq = heap_size = 0;
        ifs.open(file);
        while(ifs >> page)
            lfu(stoi(page));
        cout << f << "\t" << hit << "\t\t" << miss << "\t\t" << fixed << setprecision(10) << (double)miss/(miss + hit) << '\n';
        delete []table;
        delete []harr;
        ifs.close();
    }
    gettimeofday(&end, 0);
    double sec = end.tv_sec - start.tv_sec, usec = end.tv_usec - start.tv_usec;
    cout << "Total elapsed time " << fixed << setprecision(4) << sec + usec / 1000000 << " sec\n\n";
}

void swap2(pair<int, int> *x, pair<int, int> *y){
    pair<int, int> temp = *x;
    *x = *y;
    *y = temp;
}

void MinHeapify2(int i){
    int l = 2*i + 1;
    int r = 2*i + 2;
    int smallest = i;
    if(l < heap_size && harr2[l].second < harr2[i].second)
        smallest = l;
    if(r < heap_size && harr2[r].second < harr2[smallest].second)
        smallest = r;
    if(smallest != i){
        swap2(&harr2[i], &harr2[smallest]);
        MinHeapify2(smallest);
    }
}

pair<int, int> extractMin2(){
    if (heap_size == 1){
        heap_size--;
        return harr2[0];
    }
    pair<int, int> root = harr2[0];
    harr2[0] = harr2[heap_size-1];
    heap_size--;
    MinHeapify2(0);
    return root;
}

void insertKey2(pair<int, int> k){
    heap_size++;
    int i = heap_size - 1;
    harr2[i] = k;
    while (i != 0 && harr2[parent(i)].second > harr2[i].second){
       swap2(&harr2[i], &harr2[parent(i)]);
       i = parent(i);
    }
}

void lru(int key){
    int index = key % f;
    list<int>::iterator i;
    for(i = table[index].begin(); i != table[index].end(); i++)
        if(*i == key)
            break;
    if(i != table[index].end()){
        hit++;
        for(int j = 0; j < heap_size; j++)
            if(harr2[j].first == key){
                harr2[j].second = seq++;
                MinHeapify2(j);
                break;
            }
    }
    else{
        miss++;
        if(heap_size == f){
            pair<int, int> p = harr2[0];
            extractMin2();
            list<int>::iterator it;
            int dindex = p.first % f;
            for(it = table[dindex].begin(); it != table[dindex].end(); it++)
                if(*it == p.first){
                    table[dindex].erase(it);
                    break;
                }
            table[index].push_back(key);
            insertKey2(make_pair(key, seq++));
        }
        else{
            table[index].push_back(key);
            insertKey2(make_pair(key, seq++));
        }
    }
}

void LRU(char* file){
    struct timeval start, end;
    gettimeofday(&start, 0);
    cout << "LRU policy:\nFrame\tHit\t\tMiss\t\tPage fault ratio\n";
    for(f = 64; f <= 512; f *= 2){
        string page;
        fstream ifs;
        table = new list<int>[f];
        harr2 = new pair<int, int>[f];
        hit = miss = seq = heap_size = 0;
        ifs.open(file);
        while(ifs >> page)
            lru(stoi(page));
        cout << f << "\t" << hit << "\t\t" << miss << "\t\t" << fixed << setprecision(10) << (double)miss/(miss + hit) << '\n';
        delete []table;
        delete []harr2;
        ifs.close();
    }
    gettimeofday(&end, 0);
    double sec = end.tv_sec - start.tv_sec, usec = end.tv_usec - start.tv_usec;
    cout << "Total elapsed time " << fixed << setprecision(4) << sec + usec / 1000000 << " sec\n";
}

int main(int argc, char* argv[]){
    LFU(argv[1]);
    LRU(argv[1]);
    return 0;
}