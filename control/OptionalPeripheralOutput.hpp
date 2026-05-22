#pragma once

#include <eeros/control/PeripheralOutput.hpp>
#include <eeros/logger/Logger.hpp>
#include <optional>

template < typename T = double >
class OptionalPeripheralOutput : public eeros::control::Blockio<1,0,T> {
    std::optional<eeros::control::PeripheralOutput<T>> out;

public:
    OptionalPeripheralOutput(std::string id, bool exclusive = true) {
        try {
            out.emplace(id, exclusive);
        } catch (eeros::Fault) {
            eeros::logger::Logger::getLogger('H').warn() << "optional output " << id << " not in HAL";
        }
    }

    virtual void run() {
        if(out) out->run();
    }

    virtual T getValue() {
        if (out) return out->getValue();
        else return {};
    }

    template<typename ... ArgTypesOut>
    void callOutputFeature(std::string featureName, ArgTypesOut... args){
        if(out) out->callOutputFeature(featureName, args...);
    }
};
