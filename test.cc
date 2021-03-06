// test to delete a key
#include "cache.hh"
#include <functional>
#include <memory>
#include <string>
#include <assert.h>

std::string key = "foo";
Cache::val_type val = "bar";
Cache::val_type val2 = "car";
Cache::index_type size = 3;

// Our Hash Function, implemented as a functor by overloading the operator ().
// The algorithm is:
// - Split the string key into an array of characters
// - Iterate over the array and take the product of the prime numbers that correspond to each ASCII char
//    (e.g. 'C' is 67th on the ASCII table, and the 67th prime is 317)
// Once we have the product, return it as the bucket number to store value associated with the key
// Since any given integer is a product of primes, the only K-V pairs sharing the same bucket would be those whose keys are permutations of each other
struct MyHashFunc {
    // An array of the first 255 primes
    const uint32_t PRIME_NUMBERS[255] = {  2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89,
                                        97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 
                                        193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
                                        307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 
                                        421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 
                                        547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 
                                        661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 
                                        811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 
                                        947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 
                                        1063, 1069, 1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 
                                        1193, 1201, 1213, 1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291, 1297, 1301, 1303, 
                                        1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 
                                        1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511, 1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 
                                        1583, 1597, 1601, 1607, 1609, 1613};

    uint32_t operator() (Cache::key_type &key) const {
        const uint32_t keyLength = key.length();
        char keyArr[keyLength];
        strcpy(keyArr, key.c_str());
        uint32_t hashVal = 1;
        for (uint32_t i=0; i < keyLength; i++) {
            uint32_t charNum = keyArr[i];
            hashVal *= PRIME_NUMBERS[charNum];
        }
        return hashVal;
    }
};

void test_delete() {
    Cache c(10);
    c.set(key, val, size);
    c.del(key);
    Cache::val_type val = c.get(key,size);
    assert(val==nullptr);
}

void  test_insert() {
    Cache c(10);
    c.set(key, val, size);
    Cache::val_type get_val = c.get(key, size);
    assert(val==get_val);
}

void test_del_nonexistent() {
    Cache c(10);
    c.del(key);
    // what should happen here
    //assert(val==nullptr);
}

void  test_get_nonexistent() {
    Cache c(10);
    Cache::val_type get_val = c.get(key, size);
    assert(get_val==nullptr);
}


void test_get_modified() {
    Cache c(10);
    c.set(key, val, size);
    c.set(key, val2, size);
    Cache::val_type get_val = c.get(key, size);
    assert(get_val==val2);
}

/*
void test_get_evicted() {
    
}
*/

int main() {
    test_delete();
    test_insert();
    test_del_nonexistent();
    test_get_nonexistent();
    test_get_modified();
    return 0;
}