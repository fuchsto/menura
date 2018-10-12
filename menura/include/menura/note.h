#ifndef MENURA__NOTE_OF_FREQ_H__INCLUDED
#define MENURA__NOTE_OF_FREQ_H__INCLUDED

#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include "pa_stream.h"

namespace menura {

  enum pitch_class {
    C,       // B_#, D_bb
    C_sharp, // D_b, B_x
    D,       // E_bb, C_x
    D_sharp, // E_b, F_bb
    E,       // F_b, D_x
    F,       // E_#, G_bb
    F_sharp, // G_b, E_x
    G,       // F_x, A_bb
    G_sharp, // A_b
    A,       // G_x, B_bb
    A_sharp, // B_b, C_bb
    B        // A_x, C_b
  };

  // enum accidental {
  //   natural,
  //   sharp,        // #
  //   flat,         // b
  //   double_sharp, // x
  //   double_flat   // bb
  // };

  class note {
  public:
    note() = delete;
    note(double frequency, double pitch_class = 440.0) {
      // start off from A4:
      *this = note(pitch_class::A, 4);

      if (frequency != 0) {
        *this += 12 * std::log2(frequency / pitch_hz);
      }
      double ideal_freq = pitch_hz
                          * std::pow(
                              2.0,
                              (note_idx - a4_idx) / 12.0);

      int cent_idx = 0;
      if(frequency != 0) {
        cent_idx = 1200 * std::log2(frequency / ideal_freq);
      }

      if(frequency >= ideal_freq) {
        if(cent_idx > 50) {
          *this++;
          cent_idx = 100 - cent_idx;
          if(cent_idx != 0)
            cent_idx = (-1) * cent_idx;
        }
      } else {
        if(cent_idx >= 50) {
          *this--;
          cent_idx = 100 - cent_idx;
        } else {
          if(cent_idx != 0)
            cent_idx = (-1) * cent_idx;
        }
      }
    }

    note(pitch p, uint8_t octave)
       : _pitch(p), _octave(octave) {  }

    note(const note & other)
       : _pitch(other.p), _octave(other.octave) {  }

    note & operator=(const note & rhs) {
      _pitch  = rhs.pitch;
      _octave = rhs.octave;
    }

    // `semitone` Halbtöne höher
    note & operator+=(int semitone) {
       if (_pitch == B) {
         _octave++; _pitch = C;
       }
       else {
         _pitch = static_cast<pitch_class>(static_cast<int>(_pitch) + 1)
       }
       return *this;
    }
    note & operator++() {
      return *this += 1;
    }
    
    // `semitone` Halbtöne tiefer
    note & operator-=(int semitone) {
       if (_pitch == pitch_class::C) {
         _octave--;
         _pitch = pitch_class::B;
       }
       else {
         _pitch = static_cast<pitch_class>(static_cast<int>(_pitch) - 1)
       }
       return *this;
    }
    note & operator--() {
      return *this -= 1;
    }

    bool operator==(const note & rhs) const {
      return    _pitch  == rhs._pitch
             && _octave == rhs._octave;
    }
    bool operator!=(const note & rhs) const {
      return !(*this == rhs);
    }

    bool operator<(const note & rhs) const {
      return _octave < rhs._octave ||
             (   _octave == rhs._octave 
              && _pitch   < rhs._pitch);
    }
    bool operator<=(const note & rhs) const {
      return *this == rhs || *this < rhs;
    }

    bool operator>(const note & rhs) const {
      return *this != rhs &&
             !(*this < rhs);
    }
    bool operator>=(const note & rhs) const {
      return *this == rhs || *this > rhs;
    }

  private:
     pitch_class  _pitch;
     uint8_t      _octave;
  };

  std::ostream& operator<<(std::ostream& lhs, menura::note n);

  void note_setup();

  int midi_number_of(note n);

  note note_of(double frequency, double pitch_hz = 440.0);

  static std::vector<note> notes;

} // namespace menura

#endif // MENURA__NOTE_OF_FREQ_H__INCLUDED
