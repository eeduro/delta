#pragma once

namespace eeduro {
namespace delta {
  
// Math constants
constexpr double pi = 3.14159265359;

// General constants
constexpr unsigned int nofAxis = 4;

// Electrical and mechanical parameters
constexpr double i1524 = 76.0;	// gear ratio
constexpr double i0816 = 319333.0/2673.0;	// gear ratio for the 4th axis
constexpr double kM1524 = 11.4e-3; // [NM/A]
constexpr double kM0816 = 8.98e-3; // [Nm/A]
constexpr double RA1524 = 19.8; // [Ohm]
constexpr double RA0816 = 106.4; // [Ohm]
constexpr double jred = 0.000515154; // [kgm^2]
constexpr double mtcp = 77.2e-3; // [kg]

// Controller parameters
constexpr double dt = 0.001; // [s]
constexpr double D = 0.7;
constexpr double w0 = 2 * pi * 30; // = 2 * pi * 1 / (2 * D * dt * pi);
constexpr double kp = w0 / (2 * D);
constexpr double kd = 2 * D * w0;

constexpr double q012InitVoltage = 3; //[V]
constexpr double q3InitVoltage = 5; //[V]

constexpr double tcpReady_x = 0;
constexpr double tcpReady_y = 0;
constexpr double tcpReady_z = -0.02;
constexpr double tcpReady_phi = 1.0;

constexpr double q012homingOffset = 0.45;
constexpr double q3homingOffset = pi - 0.06;

constexpr double q012gearTorqueLimit = 0.3;

constexpr double circleRadius = 0.05;
constexpr double q3gearTorqueLimit = 0.035;

}
}
