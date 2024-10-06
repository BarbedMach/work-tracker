import WorkLogList;
import Utility;

import std;

static auto printHelp() -> void {
    std::cout
        << "Work Tracker CLI Tool\n"
        << "Usage: wt [options]\n\n"
        << "Options:\n"
        << "  --help\n"
        << R"(  --add [date{YYYY-MM-DD}] [startTime{HH:MM}] [endTime{HH:MM}] [description{"Description of the work"}])" << "\n"
        << R"(  --remove [id{task_id}])" << "\n"
        << "  --list       List all logged work\n";
}

auto main(int argc, char* argv[]) -> int {
    auto saveFile{ "work_tracker_save.txt" };

    auto list{ WorkLogList() };
    list.readFromFile(saveFile);

    auto nextId{ list.size() };

    if (argc > 1) {
        std::string arg = argv[1];

        if (arg == "--help") {
            printHelp();
            return 0;
        }
        else if (arg == "--add") {
            if (argc < 6) {
                std::println(std::cerr, "Insufficient arguments for --add. Use '--help' for usage.");
                return 1;
            }

            std::string date = argv[2];         
            std::string startTime = argv[3];    
            std::string endTime = argv[4];      
            std::string description = argv[5];

            WorkLogItem newEntry{ nextId, stringToDate(date), stringToTime(startTime), stringToTime(endTime), description};

            list.push_back(newEntry);
            std::println("Added new work log entry");

            list.writeToFile(saveFile);
        }
        else if (arg == "--remove") {
            if (argc < 3) {
                std::println(std::cerr, "Insufficient arguments for --remove. Use '--help' for usage.");
                return 1;
            }

            auto id = std::stoul(argv[2]);
            try {
                list.erase(id);
                std::println("Removed work log entry with ID: {}.", id);
                list.writeToFile(saveFile);
            }
            catch (const std::out_of_range& e) {
                std::println(std::cerr, "Error: {}", e.what());
                return 1;
            }
        }
        else if (arg == "--list") {
            std::println("{}", list);
        }
        else {
            std::println(std::cerr, "Unknown option: {}\nUse '--help' to see available options.", arg);
            return 1;
        }
    }
    else {
        std::println("No arguments provided. Use '--help' to see available options.");
        return 1;
    }

    return 0;
}