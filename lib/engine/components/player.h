#include "../game_engine.h"

class Player: public game_engine::entity {
    
    public:
        
        
        void init() override {
            this->set_name("Player");
        }

    private:

};