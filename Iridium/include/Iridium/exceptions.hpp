#ifndef IRIDIUM_EXCEPTIONS_HPP_
#define IRIDIUM_EXCEPTIONS_HPP_

#include <exception>

namespace ir {
	namespace Exceptions {
		class InvalidRenderTarget : public std::exception {
		public:
			const char * what() { return "Attempted to use invalid render target"; }
		};
		
		class BadAssetHandle : public std::exception {
		public:
			BadAssetHandle(std::string what) : what_ { what } {}

			const char * what() { return std::string{ "Requested resource ID \"" + what_ + "\" does not exist" }.c_str(); }

		private:
			std::string what_ {};
		};
		
		class BadAssetType : public std::exception {
		public:
			BadAssetType(std::string what) : what_ { what } {}

			const char * what() { return std::string{ "Provided type \"" + what_ + "\" is not an asset type" }.c_str(); }

		private:
			std::string what_ {};
		};
		
		class BadStateID : public std::exception {
		public:
			BadStateID(std::string what) : what_ { what } {}

			const char * what() { return std::string{ "State machine attempted to update nonexistent state \"" + what_ + "\"" }.c_str(); }

		private:
			std::string what_ {};
		};
		
		class InitializationError : public std::exception {
		public:
			InitializationError(std::string what) : what_ { what } {}

			const char * what() { return std::string{ what_ + " failed to initialize properly" }.c_str(); }

		private:
			std::string what_ {};
		};
		
		class BadModelName : public std::exception {
		public:
			BadModelName(std::string what) : what_ { "Attempted to load unknown VMF file \"" + what + "\"" } {}

			const char * what() { return what_.c_str(); }

		private:
			std::string what_ {};
		};
	}
}

#endif // IRIDIUM_EXCEPTIONS_HPP_