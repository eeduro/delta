#pragma once

#include <eeros/control/Block1i.hpp>
#include <eeros/logger/LogWriter.hpp>
#include <eeros/logger/Logger.hpp>

#include <string>

using namespace eeros::control;
using namespace eeros::logger;

namespace eeduro {
	namespace delta{
		template <typename T = double>
		class LogScope : public Block1i<T> {
			public:
				LogScope(std::string prefix, LogWriter* w = nullptr) {
					if(w != nullptr) log.set(w);
				}
				
				virtual void run() {
					log.info() << prefix << this->in.getSignal().getValue() << " (" << this->in.getSignal().getTimestamp() << ")";
				}
				
			private:
				Logger<LogWriter> log;
				std::string prefix;
		};
	}
};