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
        std::make_pair("max_file_size", Directives::MAX_FILE_SIZE),
        std::make_pair("time_out", Directives::TIME_OUT),
        std::make_pair("location", Directives::LOCATION),
        std::make_pair("root", Directives::ROOT),
        std::make_pair("allowed_method", Directives::ALLOWED_METHODS),
        std::make_pair("index", Directives::INDEX),
        std::make_pair("cgi", Directives::CGI),
        std::make_pair("return", Directives::REDIRECT),
        std::make_pair("upload_pass", Directives::UPLOAD),
        std::make_pair("auto_index", Directives::AUTO_INDEX),
        std::make_pair("auth_basic", Directives::AUTH_BASIC),
        std::make_pair("]", Directives::LOCATION_END),
        std::make_pair("}", Directives::SERVER_END),
    };

    for (int i = 0; i < globalDirectives.size(); ++i)
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
                }
            } else
                continue;

            switch (directive)
            {
            case -1:
                globalConfig.push_back(ServerConfig());
                ++i;
                j = -1;
                break;
            case Directives::PORT:
                if (tokens.size() != 2)
                    exitError("error near directive: <" + tokens[0] + ">");
                if (!isLocation)
                    fillGlobalDirectives(globalConfig[i]._port, tokens[1], tokens[0]);
                else
                    fillGlobalDirectives(globalConfig[i]._location[j]._port, tokens[1], tokens[0]);
                break;
            case Directives::HOST:
                if (tokens.size() != 2)
                    exitError("error near directive: <" + tokens[0] + ">");
                if (tokens[1] == "localhost")
					tokens[1] = "127.0.0.1";
				if (!isLocation)
                    fillGlobalDirectives(globalConfig[i]._host, tokens[1], tokens[0]);
                else
                    fillGlobalDirectives(globalConfig[i]._location[j]._host, tokens[1], tokens[0]);
                break;
            case Directives::SERVER_NAME:
                if (tokens.size() != 2)
                    exitError("error near directive: <" + tokens[0] + ">");
                if (!isLocation)
                    fillGlobalDirectives(globalConfig[i]._server_name, tokens[1], tokens[0]);
                else
                    fillGlobalDirectives(globalConfig[i]._location[j]._server_name, tokens[1], tokens[0]);
                break;
            case Directives::ERROR_PAGE:
                if (tokens.size() <= 2 || tokens[1] != "<" || tokens[tokens.size() - 1] != ">")
                    exitError("error near directive: <" + tokens[0] + ">");
                for (int k = 2; k < tokens.size(); ++k)
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
                break;
            case Directives::MAX_FILE_SIZE:
                if (tokens.size() != 2)
                    exitError("error near directive: <" + tokens[0] + ">");
                if (!isLocation)
                    fillGlobalDirectives(globalConfig[i]._max_file_size, tokens[1], tokens[0]);
                else
                    fillGlobalDirectives(globalConfig[i]._location[j]._max_file_size, tokens[1], tokens[0]);
                break;

            case Directives::TIME_OUT:
                if (tokens.size() != 2)
                    exitError("error near directive: <" + tokens[0] + ">");
                if (!isLocation)
                    fillGlobalDirectives(globalConfig[i]._time_out, tokens[1], tokens[0]);
                else
                    fillGlobalDirectives(globalConfig[i]._location[j]._time_out, tokens[1], tokens[0]);
                break;

            case Directives::ROOT:
                if (tokens.size() != 2)
                    exitError("error near directive: <" + tokens[0] + ">");
                if (!isLocation)
                    fillGlobalDirectives(globalConfig[i]._root, tokens[1], tokens[0]);
                else
                    fillGlobalDirectives(globalConfig[i]._location[j]._root, tokens[1], tokens[0]);
                break;

            case Directives::ALLOWED_METHODS:
                if (!isLocation)
                    for (int k = 1; k < tokens.size(); ++k)
                        globalConfig[i]._allowed_method.insert(tokens[k]);
                else
                    for (int k = 1; k < tokens.size(); ++k)
                        globalConfig[i]._location[j]._allowed_method.insert(tokens[k]);
                break;

            case Directives::REDIRECT: // Handle the redirection directive.
                if (tokens.size() != 3)
                    exitError("error near directive: <redirect>");
                globalConfig[i]._location[j]._redirect.first = tokens[1];
                globalConfig[i]._location[j]._redirect.second = tokens[2];
                break;

            case Directives::CGI: // Handle the cgi directive.
                if (tokens.size() != 2)
                    exitError("error near directive: <cgi>");
                globalConfig[i]._location[j]._cgi = tokens[1];
                break;

            case Directives::INDEX:
                if (!isLocation)
                    std::copy(tokens.begin() + 1, tokens.end(), std::back_inserter(globalConfig[i]._index));
                else
                    std::copy(tokens.begin() + 1, tokens.end(),
                              std::back_inserter(globalConfig[i]._location[j]._index));
                break;

            case Directives::AUTO_INDEX:
                if (tokens.size() != 2)
                    exitError("error near directive: <" + tokens[0] + ">");
                if (!isLocation)
                    fillGlobalDirectives(globalConfig[i]._auto_index, tokens[1], tokens[0]);
                else
                    fillGlobalDirectives(globalConfig[i]._location[j]._auto_index, tokens[1], tokens[0]);
                break;

            case Directives::LOCATION:
                if (tokens.size() != 3 || tokens[2] != "[")
                    exitError("erorr near token " + tokens[0]);
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
            case e_error::INVALID_DIRECTIVE:
                exitError("Invalid Directive " + tokens[0]);
            default:
                break;
                // exitError("Invalid directive found in config file");
            }
            //            std::cout << tokens << std::endl;
        }
    }
    else
        exitError("wrong config file path");

    if ((N_curly_braces_open != N_curly_braces_close) || (N_square_bracket_open != N_square_bracket_close))
        exitError("syntax error: config block is not well structured.");
    // std::cout << globalConfig << std::endl;
    return globalConfig;
}
