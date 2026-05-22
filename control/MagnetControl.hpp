#pragma once

#include <eeros/control/Block.hpp>
#include <eeros/control/Input.hpp>
#include <eeros/control/Output.hpp>

namespace eeduro {
namespace delta {
    class MagnetControl : public eeros::control::Block {
        using In = eeros::control::Input<bool>;
        using Out = eeros::control::Output<bool>;
        static constexpr int invertedCount = 6;
        static constexpr int deadtimeCount = 1;
        In in;
        Out outA;
        Out outB;
        bool last = false;
        bool busy = false;
        int count = 0;

        enum class State {
            Off,
            On,
            Inverted,
            Deadtime
        };

        State state = State::Off;

        public:
        virtual void run() override {
            bool current = in.getSignal().getValue();
            bool a, b;
            switch (state) {
                case State::Off:
                a = false;
                b = false;
                busy = false;
                if(current) state = State::On;
                break;
                case State::On:
                a = true;
                b = false;
                busy = true;
                if(!current) {
                    count = invertedCount;
                    state = State::Inverted;
                }
                break;
                case State::Inverted:
                a = false;
                b = true;
                busy = true;
                if(count == 0) {
                    count = deadtimeCount;
                    state = State::Deadtime;
                }
                else {
                    --count;
                }
                break;
                case State::Deadtime:
                a = false;
                b = false;
                busy = true;
                if(count == 0) {
                    state = current ? State:: On : State::Off;
                } else {
                    --count;
                }
                break;
            }
            outA.getSignal().setValue(a);
            outB.getSignal().setValue(b);
            last = current;
        }

        In& getIn() {
            return in;
        }
        Out& getOutA() {
            return outA;
        }

        Out& getOutB() {
            return outB;
        }

        bool isMagnetBusy() {
            return busy;
        }


    };
}
}
