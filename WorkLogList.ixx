export module WorkLogList;

export import WorkLogItem;
import Utility;
import std;

export class WorkLogList {
public:
	[[nodiscard]] auto operator[](std::size_t index) noexcept -> WorkLogItem& {
		return m_list[index];
	}

	[[nodiscard]] auto operator[](std::size_t index) const noexcept -> const WorkLogItem& {
		return m_list[index];
	}

	[[nodiscard]] auto getUnderlyingList() const noexcept -> const std::vector<WorkLogItem>& {
		return m_list;
	}

	[[nodiscard]] auto size() const noexcept -> const std::size_t& {
		return m_list.size();
	}

	[[nodiscard]] auto erase(std::size_t index) -> void {
		if (index < m_list.size()) {
			m_list.erase(m_list.begin() + index);
		}
		else {
			throw std::out_of_range("Index out of range");
		}
	}

	template<typename T>
	[[nodiscard]] auto push_back(T&& workLogItem) noexcept -> void {
		m_list.push_back(std::forward<T>(workLogItem));
	}

	auto readFromFile(const std::filesystem::path& pathToFile) noexcept -> bool {
		auto inFile{ std::ifstream{pathToFile} };

		if (!inFile) {
			std::println(std::cerr, "Couldn't open the specified file at the location : {}", pathToFile.string());
			std::println(std::cerr, "Creating the file: {}", pathToFile.string());

			std::ofstream newFile(pathToFile);

			if (newFile) {
				return true;
			}
			
			std::println(std::cerr, "Couldn't create the specified file at the location : {}", pathToFile.string());
			return false;
		}

		std::string line{};
		while (std::getline(inFile, line)) {
			std::istringstream inputStringStream{ line };

			std::string idStr;
			std::string dateStr;
			std::string startTimeStr;
			std::string endTimeStr;
			std::string description;

			inputStringStream >> idStr >> dateStr >> startTimeStr >> endTimeStr;

			std::getline(inputStringStream, description);

			if (!description.empty() && description[0] == ' ') {
				description.erase(0, 1);
			}

			auto id{ std::stoul(idStr) };
			auto date{ stringToDate(dateStr) };
			auto startTime{ stringToTime(startTimeStr) };
			auto endTime{ stringToTime(endTimeStr) };

			m_list.emplace_back(id, date, startTime, endTime, description);
		}
	}

	auto writeToFile(const std::filesystem::path& pathToFile) const noexcept -> bool {
		auto outFile{ std::ofstream{pathToFile} };

		if (!outFile) {
			std::cerr << "Couldn't open the specified file at the location : " << pathToFile << std::endl;
			return false;
		}

		for (const auto& item : m_list) {
			const auto& date = item.getDate();

			const auto formattedDate{ std::format("{:%F}", date) };

			const auto& startTime = item.getStartTime();
			const auto& endTime = item.getEndTime();

			const auto formattedStartTime{ std::format("{:%R}", startTime) };
			const auto formattedEndTime{ std::format("{:%R}", endTime) };

			outFile << item.getId() << " " << formattedDate << " " << formattedStartTime << " " << formattedEndTime << " " << item.getDescription() << "\n";
		}
	}
private:
	std::vector<WorkLogItem> m_list{};
};

export template<> struct std::formatter<WorkLogList> {
	constexpr auto parse(std::format_parse_context& context) {
		return context.begin();
	}

	template <typename FormatContext>
	auto format(const WorkLogList& workLogList, FormatContext& context) const {
		auto out = context.out();

		for (const auto& item : workLogList.getUnderlyingList()) {
			out = std::format_to(out, "{}\n", item);
		}

		return out;
	}
};