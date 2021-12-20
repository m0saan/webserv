#include "../includes/parser.hpp"
#include "../includes/utility.hpp"

void exitError(std::string const &error)
{
    std::cout << error << std::endl;
    exit(EXIT_FAILURE);
}

int getDirective(std::string const &token)
{
    static const std::array<std::pair<std::string, int>, 17> globalDirectives = {
        std::make_pair("port", Directives::PORT),
        std::make_pair("host", Directives::HOST),
        std::make_pair("server_name", Directives::SERVER_NAME),
        std::make_pair("error_page", Directives::ERROR_PAGE),
        std::make_pair("max_body_size", Directives::MAX_FILE_SIZE),
        std::make_pair("time_out", Directives::TIME_OUT),
        std::make_pair("location", Directives::LOCATION),
        std::make_pair("root", Directives::ROOT),
        std::make_pair("allowed_method", Directives::ALLOWED_METHODS),
        std::make_pair("index", Directives::INDEX),
        std::make_pair("cgi", Directives::CGI),
        std::make_pair("return", Directives::REDIRECT),
        std::make_pair("upload_store", Directives::UPLOAD),
        std::make_pair("auto_index", Directives::AUTO_INDEX),
        std::make_pair("auth_basic", Directives::AUTH_BASIC),
        std::make_pair("]", Directives::LOCATION_END),
        std::make_pair("}", Directives::SERVER_END),
    };

    for (size_t i = 0; i < globalDirectives.size(); ++i)
        if (token == globalDirectives[i].first)
            return globalDirectives[i].second;
    return INVALID_DIRECTIVE;
}

template <typename T>
void fillGlobalDirectives(T &field, T const &value, std::string const &directive)
{
    if (field != "")
        exitError("Error: Invalid directive: <" + directive + ">");
    field = value;
}

void getPort(std::vector<std::string> const &tokens, bool const &isLocation, std::vector<ServerConfig> &globalConfig, int i, int j)
{
    if (tokens.size() != 2)
        exitError("error near directive: <" + tokens[0] + ">");
    if (!isLocation)
        fillGlobalDirectives(globalConfig[i]._port, tokens[1], tokens[0]);
    else
        fillGlobalDirectives(globalConfig[i]._location[j]._port, tokens[1], tokens[0]);
}

void getHost(std::vector<std::string> &tokens, bool const &isLocation, std::vector<ServerConfig> &globalConfig, int i, int j)
{
    if (tokens.size() != 2)
        exitError("error near directive: <" + tokens[0] + ">");
    if (tokens[1] == "localhost")
        tokens[1] = "127.0.0.1";
    if (!isLocation)
        fillGlobalDirectives(globalConfig[i]._host, tokens[1], tokens[0]);
    else
        fillGlobalDirectives(globalConfig[i]._location[j]._host, tokens[1], tokens[0]);
}

void getServerName(std::vector<std::string> &tokens, bool const &isLocation, std::vector<ServerConfig> &globalConfig, int i, int j)
{
    if (tokens.size() != 2 || !globalConfig[i]._server_name.empty())
        exitError("error near directive: <" + tokens[0] + ">");
    if (!isLocation)
        fillGlobalDirectives(globalConfig[i]._server_name, tokens[1], tokens[0]);
    else
        fillGlobalDirectives(globalConfig[i]._location[j]._server_name, tokens[1], tokens[0]);
}

void getErrorPage(std::vector<std::string> &tokens, std::vector<ServerConfig> &globalConfig, int i)
{
    if (tokens.size() <= 2 || tokens[1] != "<" || tokens[tokens.size() - 1] != ">" || !globalConfig[i]._error_page.empty())
        exitError("error near directive: <" + tokens[0] + ">");
    for (size_t k = 2; k < tokens.size(); ++k)
    {
        if (tokens[k] == ">")
            break;
        std::vector<std::string> res = Utility::split(tokens[k], ':');
        if (res.size() != 2)
            exitError("error near directive: <" + tokens[0] + ">");
        if (res[1].back() != ';')
            exitError("<;> delimiter is expected.");
        res[1].pop_back();
        globalConfig[i]._error_page[res[0]] = res[1];
    }
}

void getMaxFileSize(std::vector<std::string> &tokens, bool const &isLocation, std::vector<ServerConfig> &globalConfig, int i, int j)
{
    if (tokens.size() != 2)
        exitError("error near directive: <" + tokens[0] + ">");
    if (!isLocation)
        fillGlobalDirectives(globalConfig[i]._max_file_size, tokens[1], tokens[0]);
    else
        fillGlobalDirectives(globalConfig[i]._location[j]._max_file_size, tokens[1], tokens[0]);
}

void getTimeOut(std::vector<std::string> &tokens, bool const &isLocation, std::vector<ServerConfig> &globalConfig, int i, int j)
{
    if (tokens.size() != 2)
        exitError("error near directive: <" + tokens[0] + ">");
    if (!isLocation)
        fillGlobalDirectives(globalConfig[i]._time_out, tokens[1], tokens[0]);
    else
        fillGlobalDirectives(globalConfig[i]._location[j]._time_out, tokens[1], tokens[0]);
}

void getRoot(std::vector<std::string> &tokens, bool const &isLocation, std::vector<ServerConfig> &globalConfig, int i, int j)
{
    if (tokens.size() != 2)
        exitError("error near directive: <" + tokens[0] + ">");
    if (!isLocation)
    {
        if (!globalConfig[i]._root.empty())
            exitError("error near directive: <" + tokens[0] + ">");
        fillGlobalDirectives(globalConfig[i]._root, tokens[1], tokens[0]);
    }
    else
    {
        if (!globalConfig[i]._location[j]._root.empty())
            exitError("error near directive: <" + tokens[0] + ">");
        fillGlobalDirectives(globalConfig[i]._location[j]._root, tokens[1], tokens[0]);
    }
}

void getAllowedMethods(std::vector<std::string> &tokens, bool const &isLocation, std::vector<ServerConfig> &globalConfig, int i, int j)
{
    if (!isLocation)
    {

        if (!globalConfig[i]._allowed_method.empty())
            exitError("error near directive: <" + tokens[0] + ">");

        for (size_t k = 1; k < tokens.size(); ++k)
            globalConfig[i]._allowed_method.insert(tokens[k]);
    }
    else
    {

        if (!globalConfig[i]._location[j]._allowed_method.empty())
            exitError("error near directive: <" + tokens[0] + ">");
        for (size_t k = 1; k < tokens.size(); ++k)
            globalConfig[i]._location[j]._allowed_method.insert(tokens[k]);
    }
}

void getRedirect(std::vector<std::string> &tokens, std::vector<ServerConfig> &globalConfig, int i, int j)
{
    if (tokens.size() != 3 || !globalConfig[i]._location[j]._redirect.first.empty())
        exitError("error near directive: <redirect>");
    globalConfig[i]._location[j]._redirect.first = tokens[1];
    globalConfig[i]._location[j]._redirect.second = tokens[2];
}

void getUpload(std::vector<std::string> &tokens, std::vector<ServerConfig> &globalConfig, int i, int j)
{
    if (tokens.size() != 2 || !globalConfig[i]._location[j]._upload_store.empty())
        exitError("error near directive: <upload>");
    globalConfig[i]._location[j]._upload_store = tokens[1];
}

void getCGI(std::vector<std::string> &tokens, std::vector<ServerConfig> &globalConfig, int i, int j)
{
    if (tokens.size() != 2 || !globalConfig[i]._location[j]._cgi.empty())
        exitError("error near directive: <cgi>");
    globalConfig[i]._location[j]._cgi = tokens[1];
}

void getIndex(std::vector<std::string> &tokens, bool const &isLocation, std::vector<ServerConfig> &globalConfig, int i, int j)
{
    if (!isLocation)
    {
        std::copy(tokens.begin() + 1, tokens.end(), std::back_inserter(globalConfig[i]._index));
        if (!globalConfig[i]._index.empty())
            exitError("error near directive: <" + tokens[0] + ">");
    }
    else
    {
        if (!globalConfig[i]._location[j]._index.empty())
            exitError("error near directive: <" + tokens[0] + ">");
        std::copy(tokens.begin() + 1, tokens.end(),
                  std::back_inserter(globalConfig[i]._location[j]._index));
    }
}

void getAutoIndex(std::vector<std::string> &tokens, bool const &isLocation, std::vector<ServerConfig> &globalConfig, int i, int j)
{
    if (tokens.size() != 2)
        exitError("error near directive: <" + tokens[0] + ">");
    if (!isLocation)
        fillGlobalDirectives(globalConfig[i]._auto_index, tokens[1], tokens[0]);
    else
        fillGlobalDirectives(globalConfig[i]._location[j]._auto_index, tokens[1], tokens[0]);
}

std::vector<ServerConfig> performParsing(std::string const &filename)
{
    std::vector<ServerConfig> globalConfig;
    std::ifstream ifs;
    ifs.open(filename, std::ios_base::in);

    std::string line;
    int i = -1, j = -1;
    int N_curly_braces_open = 0, N_square_bracket_open = 0;
    int N_curly_braces_close = 0, N_square_bracket_close = 0;
    bool isLocation = false;
    int directive;
    if (ifs.is_open())
    {
        while (std::getline(ifs, line))
        {
            if (line.empty())
                continue;

            std::vector<std::string> tokens = Utility::split(line);
            if (tokens.size())
            {
                directive = getDirective(tokens[0]);
                if (tokens[0] == "server")
                {
                    if (tokens.size() != 2 || tokens[1] != "{")
                        exitError("error near directive: <" + tokens[0] + ">");
                    directive = -1;
                    ++N_curly_braces_open;
                    if (N_square_bracket_close != N_square_bracket_open)
                        exitError("syntax error: config block is not well structured.");
                }
            }
            else
                continue;

            switch (directive)
            {

            case -1:
                globalConfig.push_back(ServerConfig());
                ++i;
                j = -1;
                break;

            case Directives::PORT:
                getPort(tokens, isLocation, globalConfig, i, j);
                break;

            case Directives::HOST:
                getHost(tokens, isLocation, globalConfig, i, j);
                break;

            case Directives::SERVER_NAME:
                getServerName(tokens, isLocation, globalConfig, i, j);
                break;

            case Directives::ERROR_PAGE:
                getErrorPage(tokens, globalConfig, i);
                break;

            case Directives::MAX_FILE_SIZE:
                getMaxFileSize(tokens, isLocation, globalConfig, i, j);
                break;

            case Directives::TIME_OUT:
                getTimeOut(tokens, isLocation, globalConfig, i, j);
                break;

            case Directives::ROOT:
                getRoot(tokens, isLocation, globalConfig, i, j);
                break;

            case Directives::ALLOWED_METHODS:
                getAllowedMethods(tokens, isLocation, globalConfig, i, j);
                break;

            case Directives::REDIRECT: // Handle the redirection directive.
                getRedirect(tokens, globalConfig, i, j);
                break;

            case Directives::UPLOAD: // Handle the redirection directive.
                getUpload(tokens, globalConfig, i, j);
                break;

            case Directives::CGI: // Handle the cgi directive.
                getCGI(tokens, globalConfig, i, j);
                break;

            case Directives::INDEX:
                getIndex(tokens, isLocation, globalConfig, i, j);
                break;

            case Directives::AUTO_INDEX:
                getAutoIndex(tokens, isLocation, globalConfig, i, j);
                break;

            case Directives::LOCATION:
                if (tokens.size() != 3 || tokens[2] != "[")
                    exitError("erorr near block: <" + tokens[0] + ">");
                globalConfig[i]._location.push_back(ServerConfig());
                ++j;
                globalConfig[i]._location[j]._loc_path = tokens[1];
                ++N_square_bracket_open;
                isLocation = true;
                break;

            case Directives::LOCATION_END:
                isLocation = false;
                ++N_square_bracket_close;
                break;

            case Directives::SERVER_END:
                ++N_curly_braces_close;
                break;

            case INVALID_DIRECTIVE:
                exitError("Invalid Directive " + tokens[0]);
            default:
                break;
                exitError("Invalid directive found in config file");
            }
        }
    }
    else
        exitError("wrong config file path");

    if ((N_curly_braces_open != N_curly_braces_close) || (N_square_bracket_open != N_square_bracket_close))
        exitError("syntax error: config block is not well structured.");
    return globalConfig;
}
