#include <ranges>
#include <concepts>
#include <iostream>
#include <ostream>
#include <string>


namespace prhrs {

	template<typename T>
	concept Writeable = 
		std::derived_from<T, std::ostream> ||
		std::derived_from<T, std::wostream>;

	template<Writeable Stream=std::ostream>
	class PrhrsLogger {
		private:
			Stream& stream_;
			std::size_t size_ = -1;

			int columns_ = 40;
			char block_symbol_ = '#';
			std::size_t completed_ = 0;

		public:
			PrhrsLogger()
				: stream_(std::cout) { }
			PrhrsLogger(Stream& stream)
				: stream_(stream) { }
			PrhrsLogger(std::size_t size)
				: size_(size), stream_(std::cout) { }
			PrhrsLogger(std::size_t size, Stream& stream)
				: size_(size), stream_(stream) { }
			
			void log() {
				completed_ += 1;
				if (size_ != -1) {
					double percentage = static_cast<double>(completed_) / size_;
					int columns_filled = static_cast<int>(percentage * columns_);
					stream_ << '|';
					for (int i = 0; i < columns_filled; ++i) {
						stream_ << "\u258a";
					}
					for (int i = columns_filled; i < columns_; ++i) {
						stream_ << ' ';
					}
					stream_ << '|' << '\r';
				} else {
					stream_ << '|' << std::to_string(completed_) << " it" << '\r';
				}
			}
	};


	template <std::ranges::forward_range Range, Writeable Stream=std::ostream>
	class Prhrs {
		private:
			using value_t = std::ranges::range_value_t<Range>;
			using iterator_t = std::ranges::iterator_t<Range>;
			using sentinel_t = std::ranges::sentinel_t<Range>;
			
			iterator_t it_;
			sentinel_t end_;
			PrhrsLogger<Stream> logger_;

		public:
			Prhrs(Range& range) 
				: it_(std::ranges::begin(range)), end_(std::ranges::end(range)), logger_(std::ranges::size(range)) { }
			Prhrs(const Range& range)
				: it_(std::ranges::cbegin(range)), end_(std::ranges::cend(range)), logger_(std::ranges::size(range)) { }	
			Prhrs(Range& range, Stream& stream)
				: it_(std::ranges::begin(range)), end_(std::ranges::end(range)), logger_(std::ranges::size(range), stream) { }
			Prhrs(const Range& range, Stream& stream)
				: it_(std::ranges::cbegin(range)), end_(std::ranges::cend(range)), logger_(std::ranges::size(range), stream) { }

			bool operator== (const sentinel_t& sentinel) const {
				return it_ == sentinel;
			}

			Prhrs& begin() {
				return *this;
			}
			const Prhrs& begin() const {
				return *this;
			}
			const sentinel_t end() const {
				return end_;
			}
			Prhrs& operator++() {
				++it_;
				logger_.log();
				return *this;
			}
			const Prhrs& operator++() const{
				++it_;
				logger_.log();
				return *this;
			}
			const Prhrs operator++(int) {
				auto prev = it_;
				++it_;
				logger_.log();
				return *this;
			}
			auto operator*() {
				return *it_;
			}
	};


	template <std::ranges::forward_range Range>
	auto prhrs(Range& range) {
		return Prhrs<Range>(range);
	}
	template <std::ranges::forward_range Range>
	auto prhrs(const Range& range) {
		return Prhrs<Range>(range);
	}

	template <std::ranges::forward_range Range, Writeable Stream>
	auto prhrs(Range& range, Stream& stream) {
		return Prhrs(range, stream);
	}
	template <std::ranges::forward_range Range, Writeable Stream>
	auto prhrs(const Range& range, Stream& stream) {
		return Prhrs(range, stream);
	}

	auto prange(std::size_t n) {
		return Prhrs(std::ranges::iota_view{0ul, n});
	}

	template<Writeable Stream>
	auto prange(std::size_t n, Stream& stream) {
		return Prhrs(std::ranges::iota_view{0ul, n}, stream);
	}
}
