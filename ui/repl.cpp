#include "repl.hpp"

std::string getpass_win(const char *prompt);
std::string getpass_unix(const char *prompt);
#ifdef _WIN32
std::string getpass(const char *p)
{
    return getpass_win(p);
}
#else
std::string getpass(const char *p)
{
    return getpass_unix(p);
}
#endif

bool displaySite(const MemTable &data);
bool flushData(MemTable &data);
bool findData(const MemTable &data);
bool insertData(MemTable &data);
bool deleteData(MemTable &data);
bool updateData(MemTable &data);

bool initialize()
{
    std::cout << "initializing........." << std::endl;
    std::string pw1 = getpass("Enter password: ");
    std::string pw2 = getpass("Enter password again: ");
    if (pw1 != pw2)
    {
        std::cout << "The password do not match! Please try again" << std::endl;
        return false;
    }
    std::vector<passwordData> empty;
    std::string json = serialize(empty);
    Bytes cipher = encrypt(json, pw1);
    writeFile("vault/vault-" + currentISO8601() + ".bin", cipher);
    std::cout << "initializeVault done" << std::endl;
    return true;
}
#ifdef _WIN32
std::string getpass_win(const char *prompt)
{
    std::cout << prompt << std::flush;

    const char BACKSPACE = 8;
    const char ENTER = 13;
    std::string pw;
    char ch;
    while ((ch = _getch()) != ENTER)
    {
        if (ch == BACKSPACE && !pw.empty())
        {
            pw.pop_back();
            std::cout << "\b \b";
        }
        else if (ch != BACKSPACE)
        {
            pw.push_back(ch);
            std::cout << '*';
        }
    }
    std::cout << '\n';
    return pw;
}
#else
std::string getpass_unix(const char *prompt)
{
    std::cout << prompt << std::flush;
    termios oldt{}, newt{};
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflg &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    std::string pw;
    std::getline(std::cin, pw);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << '\n';
    return pw;
}
#endif

bool accessSystem()
{
    std::cout << "Checking vault data............";
    //检查是否有文件夹存在，没有就创建
    if (!std::filesystem::exists("vault"))
    {
        std::filesystem::create_directory("vault");
    }
    std::string vaultPath = findFilePath();
    //检查是否有数据文件存在
    if (!vaultPath.empty())
    {
        std::cout << "vault exist!" << std::endl;
    }
    else
    {
        int count = 0;
        std::cout << "No data initializing......" << std::endl;
        while (!initialize())
        {
            count++;
            if (count > 10)
            {
                std::cout << "Initializing failed" << std::endl;
                return false;
            }
        };
    }
    return true;
}
void menuLogic()
{
    std::cout << "Welcome to the Password Manager!\n"
              << "Please enter the vault`s password\n";
    std::string pw1 = getpass("Enter password: ");
    std::string vaultPath = findFilePath();

    MemTable data(vaultPath, pw1);
    while (true)
    {
        system("cls");
        std::cout << "=== Main Menu ===\n";
        std::cout << "1. View Data\n";
        std::cout << "2. Flush Data\n";
        std::cout << "3. Find Data\n";
        std::cout << "4. Insert Data\n";
        std::cout << "5. Delete Data\n";
        std::cout << "6. Update Data\n";
        std::cout << "7. Exit\n";
        std::cout << "=================\n";
        int choice = 0;
        if (!(std::cin >> choice))
        {
            std::cin.clear();                                                   // 清除错误标志
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 丢弃无效输入
            std::cout << "Invalid input. Please enter a number between 1 and 7.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清除输入缓冲区

        switch (choice)
        {
        case 1:
            displaySite(data);
            waitForEnter();
            break;
        case 2:
            flushData(data);
            waitForEnter();
            break;
        case 3:
            findData(data);
            waitForEnter();
            break;
        case 4:
            insertData(data);
            waitForEnter();
            break;
        case 5:
            deleteData(data);
            waitForEnter();
            break;
        case 6:
            updateData(data);
            waitForEnter();
            break;
        case 7:
            flushData(data);
            std::cout << "Exiting application.\n";
            return;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}
bool displaySite(const MemTable &data_)
{
    const auto &allData = data_.getAll();
    if (allData.empty())
    {
        std::cout << "No data available.\n";
        return false;
    }
    std::cout << "=== Stored Data ===\n";
    for (const auto &pd : allData)
    {
        std::cout << "Site: " << pd.site << "\n";
    }
    std::cout << "===================\n";
    return true;
}
bool findData(const MemTable &data)
{
    std::cout << "Enter site to find: ";
    std::string site;
    std::cin >> site;
    const passwordData *pd = data.find(site);
    if (pd)
    {
        std::cout << "Site: " << pd->site << "\n";
        std::cout << "Username: " << pd->userName << "\n";
        std::cout << "Password: " << pd->password << "\n";
        std::cout << "Last Updated: " << pd->updatedAt << "\n";
        return true;
    }
    else
    {
        std::cout << "No entry found for site: " << site << "\n";
        return false;
    }
}
bool flushData(MemTable &data_)
{
    try
    {
        data_.flushToDisk("vault-" + currentISO8601() + ".bin");
        std::cout << "Data flushed to disk successfully.\n";
        return true;
    }
    catch (const std::exception &e)
    {
        std::cout << "Error flushing data: " << e.what() << "\n";
        return false;
    }
}
bool insertData(MemTable &data_)
{
    passwordData pd;
    std::cout << "Enter site: ";
    std::cin >> pd.site;
    std::cout << "Enter username: ";
    std::cin >> pd.userName;
    std::cout << "Enter password: ";
    std::cin >> pd.password;
    pd.updatedAt = currentISO8601();
    try
    {
        data_.insert(pd);
        std::cout << "Data inserted successfully.\n";
        return true;
    }
    catch (const std::exception &e)
    {
        std::cout << "Error inserting data: " << e.what() << "\n";
        return false;
    }
}
bool deleteData(MemTable &data_)
{
    std::cout << "Enter site to delete: ";
    std::string site;
    std::cin >> site;
    if (data_.remove(site))
    {
        std::cout << "Data deleted successfully.\n";
        return true;
    }
    else
    {
        std::cout << "No entry found for site: " << site << "\n";
        return false;
    }
}
bool updateData(MemTable &data_)
{
    passwordData pd;
    std::cout << "Enter site to update: ";
    std::cin >> pd.site;
    std::cout << "Enter new username: ";
    std::cin >> pd.userName;
    std::cout << "Enter new password: ";
    std::cin >> pd.password;
    pd.updatedAt = currentISO8601();
    try
    {
        data_.update(pd);
        std::cout << "Data updated successfully.\n";
        return true;
    }
    catch (const std::exception &e)
    {
        std::cout << "Error updating data: " << e.what() << "\n";
        return false;
    }
}
void waitForEnter()
{
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
std::string now_str()
{
    std::time_t t = std::time(nullptr);
    char buf[32]{};
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return buf;
}
void log_exception(const std::exception &e, std::ostream &os, int depth = 0)
{
    os << now_str() << "[error]" << std::string(depth * 2, ' ') << "exception(" << typeid(e).name() << "):" << e.what() << '\n';
    try
    {
        std::rethrow_if_nested(e);
    }
    catch (const std::exception &nested)
    {
        log_exception(nested, os, depth + 1);
    }
    catch (...)
    {
        os << now_str() << "[error]" << std::string((depth + 1) * 2, ' ') << "unknown nested exception\n";
    }
}
void writeLogFile(const std::exception &ex)
{
    if (std::ofstream log("crash.log", std::ios::app); log)
    {
        log_exception(ex, log);
        log << "---------------------------------------------\n";
    }
}