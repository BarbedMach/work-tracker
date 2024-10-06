export module WorkLogItem;

import std;

export using time_t = std::chrono::hh_mm_ss<std::chrono::minutes>;
export using date_t = std::chrono::year_month_day;

export class WorkLogItem {
public:
	[[nodiscard]] constexpr WorkLogItem(std::size_t id, const date_t& date, const time_t& startTime, const time_t& endTime, std::string description) noexcept 
		: m_id{id}, m_startTime{startTime}, m_endTime{endTime}, m_date{date}, m_description{std::move(description)} 
	{}

	[[nodiscard]] constexpr auto getDescription() const noexcept -> const std::string& {
		return m_description;
	}

	[[nodiscard]] constexpr auto getStartTime() const noexcept -> const time_t& {
		return m_startTime;
	}

	[[nodiscard]] constexpr auto getEndTime() const noexcept -> const time_t& {
		return m_endTime;
	}

	[[nodiscard]] constexpr auto getDate() const noexcept -> const date_t& {
		return m_date;
	}

	[[nodiscard]] constexpr auto getId() const noexcept -> const std::size_t& {
		return m_id;
	}
		
private:
	std::size_t m_id;
	time_t m_startTime;
	time_t m_endTime;
	date_t m_date;
	std::string m_description;
};

export template<> struct std::formatter<WorkLogItem> {
	constexpr auto parse(std::format_parse_context& context) {
		return context.begin();
	}

	template<typename FormatContext>
	auto format(const WorkLogItem& item, FormatContext& context) const {

		const auto& date = item.getDate();

		const auto formattedDate{ std::format("{:%F}", date) };

		const auto& startTime = item.getStartTime();
		const auto& endTime = item.getEndTime();

		const auto formattedStartTime{ std::format("{:%R}", startTime) };
		const auto formattedEndTime{ std::format("{:%R}", endTime) };

		return std::format_to(context.out(), "ID: {}\nDate: {}\nStart Time: {}\nEnd Time: {}\nDescription: {}\n", item.getId(), formattedDate, formattedStartTime, formattedEndTime, item.getDescription());
	}
};