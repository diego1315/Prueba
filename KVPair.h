#include <iostream>
#include <utility>  

using std::ostream;

/* Clase que implementa un tipo de dato abstracto KVPair, como un dato compuesto de llave y valor*/

template <typename K, typename V> 
class KVPair {
public:
    K key;
    V value;

    /* Constructores */
    KVPair() = default;
    
    KVPair(K key) : key(std::move(key)) {}
    
    KVPair(K key, V value) : key(std::move(key)), value(std::move(value)) {}
    
    /* Constructor de copia */
    KVPair(const KVPair& other) = default;
    
	/* Operadores del tipo de dato */
    // Operador de asignación por copia
    KVPair& operator=(const KVPair& other) = default;
    
    // Constructor de movimiento
    KVPair(KVPair&& other) noexcept = default;
    
    // Operador de asignación por movimiento
    KVPair& operator=(KVPair&& other) noexcept = default;
    
    // Operadores de comparación
    bool operator==(const KVPair& other) const {
        return key == other.key;
    }
    
    bool operator!=(const KVPair& other) const {
        return !(*this == other);
    }
    
    bool operator<(const KVPair& other) const {
        return key < other.key;
    }
    
    bool operator>(const KVPair& other) const {
        return key > other.key;
    }
    
    bool operator<=(const KVPair& other) const {
        return !(*this > other);
    }
    
    bool operator>=(const KVPair& other) const {
        return !(*this < other);
    }
};

/* Manejo de la impresión de un elemento KVPair */
template <typename K, typename V>
ostream& operator<<(ostream& os, const KVPair<K, V>& pair) {
    os << "(" << pair.key << ": " << pair.value << ")";
    return os;
}