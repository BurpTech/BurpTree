#pragma once

namespace BurpTree {
  namespace Internal {

    class Status {

      public:

        using Code = unsigned int;

        enum class Level {
          INFO,
          WARNING,
          ERROR
        };

        Level level;
        Code code;

        static const Status & maxLevel(const Status & first, const Status & second);
        Status();
        void set(Level level, Code code);
        const Level getLevel() const;
        const Code getCode() const;
        virtual const char * c_str() const = 0;

      protected:

        Level _level;
        Code _code;

    };

  }
}
