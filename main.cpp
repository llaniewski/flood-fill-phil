#include <stdio.h>
#include <map>
#include <queue>

const int L = 50; // Box limits


typedef size_t idx_t;
const idx_t NULL_ID = (size_t) -1;
typedef int coord_t;

coord_t d3q27_x[] = {0,-1,1,0,-1,1,0,-1,1,0,-1,1,0,-1,1,0,-1,1,0,-1,1,0,-1,1,0,-1,1};
coord_t d3q27_y[] = {0,0,0,1,1,1,-1,-1,-1,0,0,0,1,1,1,-1,-1,-1,0,0,0,1,1,1,-1,-1,-1};
coord_t d3q27_z[] = {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1};


struct Coords {
    coord_t x,y,z;
    bool operator<(const Coords& other) const {
        if (x < other.x) return true;
        if (x > other.x) return false;
        if (y < other.y) return true;
        if (y > other.y) return false;
        if (z < other.z) return true;
        return false;
    }
};

struct Node {
    Coords coords;
    idx_t id;
    idx_t con[27];
    bool finish;
};

typedef std::queue<idx_t> queue_t;
typedef std::map<Coords, idx_t> map_t;
typedef std::vector<Node> vector_t;

bool in_box(const Coords & c) {
    if (c.x <  0) return false;
    if (c.x >= L) return false;
    if (c.y <  0) return false;
    if (c.y >= L) return false;
    if (c.z <  0) return false;
    if (c.z >= L) return false;
    return true;
}

int main() {
    queue_t Q;
    map_t M;
    vector_t V;
    {
        Node n;
        n.coords.x = 0;    
        n.coords.y = 0;    
        n.coords.z = 0;
        n.id = V.size();
        V.push_back(n);
        M[n.coords] = n.id;
        Q.push(n.id);
    }
    while (! Q.empty()) {
        idx_t id = Q.front();
        for (int i=0; i<27; i++) {
            Coords c;
            c.x = V[id].coords.x + d3q27_x[i];
            c.y = V[id].coords.y + d3q27_y[i];
            c.z = V[id].coords.z + d3q27_z[i];
            map_t::iterator it = M.find(c);
            idx_t new_id = NULL_ID;
            if (it == M.end()) {
                if (! V[id].finish) {
                    Node m;
                    new_id = m.id = V.size();
                    m.coords = c;
                    m.finish = true;
                    V.push_back(m);
                    M[m.coords] = m.id;
                    Q.push(m.id);
                }
            } else {
                new_id = it->second;
            }
            V[id].con[i] = new_id;
            if (new_id != NULL_ID) {
                if (in_box(V[new_id].coords)) // <<--- checking for cuts should be somewhere here.
                    V[new_id].finish = false;
            }            
        }
        Q.pop();
    }
    printf("size: %ld\n", V.size());
    if (V.size() < 40) {
        for (size_t i=0; i<V.size(); i++) {
            printf("%ld (%3d,%3d,%3d)", i, V[i].coords.x, V[i].coords.y, V[i].coords.z);
            for (int j=0; j<27; j++) {
                printf(" %ld", V[i].con[j]);
            }
            printf("\n");
        }
    }
}
