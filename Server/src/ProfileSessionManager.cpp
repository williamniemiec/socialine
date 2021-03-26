//
// Created by Farias, Karine on 3/17/21.
//

#include "../include/ProfileSessionManager.h"
#include "../../Utils/Types.h"

using namespace std;

int ProfileSessionManager::login( std::string username, std::string session_id)
{
    cout << "Welcome to Profile and Session Manager" << NEW_LINE ;
    cout << "I am trying to log you in, " << username << NEW_LINE;

    //INICIALIZANDO O RETORNO
    int return_code = 0;

    cout << "You have " << sessions_map[username].size() << " sessions opened" << NEW_LINE;
    //PRINTANDO SESSÕES ANTES DA EXECUÇÃO
    for (int k = 0; k < sessions_map[username].size() ; ++k)
        cout << "\t" << sessions_map[username][k] << NEW_LINE;

    //LOGICA DE CONTROLE DE SESSÃO - LOGIN
    if( sessions_map[username].size() == 0 )
        return_code = create_user( username, session_id );
    else if( sessions_map[username].size() > 0 && sessions_map[username].size() < 2)
        return_code = handle_session(username, session_id);
    else{
        cout << "Error more than two open sessions"<< NEW_LINE;
    }

    cout << "You have " << sessions_map[username].size() << " sessions opened" << NEW_LINE;
    //PRINTANDO SESSÕES DEPOIS DA EXECUÇÃO
    for (int k = 0; k < sessions_map[username].size() ; ++k)
        cout << "\t" << sessions_map[username][k] << NEW_LINE;

    cout << "\n\n";

    return return_code;
}

int ProfileSessionManager::follow( std::string follower, std::string followed )
{

    std::cout << "Welcome to Profile and Session Manager" << NEW_LINE ;
    std::cout << "I am trying to follow another profile" << NEW_LINE;

    //INICIALIZANDO O RETORNO
    int return_code = 0;

    //PRINTANDO DADOS MOCKADOS - TESTE
    cout << "I am " << follower << " and I follow this accounts:" << NEW_LINE;

    for (int k = 0; k < followers_map[follower].size() ; ++k)
        cout << "\t" << followers_map[follower][k] << NEW_LINE;

    //LOGICA DE CONTROLE DE SEGUIDORES - FOLLOW
    if (follower != followed)
        followers_map[follower].push_back(followed);
    else
    {
        return_code = ERROR_FOLLOW_YOURSELF;
    }

    cout << "I am " << follower << " and now I follow this accounts:" << NEW_LINE;

    //PRINTANDO SEGUIDORES DEPOIS DA EXECUÇÃO
    for (int k = 0; k < followers_map[follower].size() ; ++k)
        cout << "\t" << followers_map[follower][k] << NEW_LINE;

    cout << "\n\n";
    return return_code;
}

int ProfileSessionManager::logout( std::string username, std::string session_id )
{
    cout << "Welcome to Profile and Session Manager" << NEW_LINE ;
    cout << "I am trying to log you out, " << username << NEW_LINE;

    //INICIALIZANDO O RETORNO
    int return_code = 0;

    cout << "You have " << sessions_map[username].size() << " sessions opened" << NEW_LINE;
    //PRINTANDO SESSÕES ANTES DA EXECUÇÃO
    for (int k = 0; k < sessions_map[username].size() ; ++k)
        cout << "\t" << sessions_map[username][k] << NEW_LINE;

    //LOGICA DE CONTROLE DE SESSÃO - LOGOUT
    if(sessions_map[username].size() == 0)
        return_code = -1;
    else
        for(int i = 0; i < sessions_map[username].size(); i++)
            if(sessions_map[username][i] == session_id )
                sessions_map[username].erase(sessions_map[username].begin()+i);

    cout << "You have " << sessions_map[username].size() << " sessions opened" << NEW_LINE;

    //PRINTANDO SESSÕES DEPOIS DA EXECUÇÃO
    for (int k = 0; k < sessions_map[username].size() ; ++k)
        cout << "\t" << sessions_map[username][k] << NEW_LINE;

    cout << "\n\n";

    return return_code;
}


int ProfileSessionManager::create_user(std::string username, std::string session_id)
{
    vector<string> followers;
    followers_map[username] = followers;

    sessions_map[username].push_back(session_id);

    return 0;
}

int ProfileSessionManager::handle_session(std::string username, std::string session_id)
{
    int return_code = 0;
    for(int i = 0; i < sessions_map[username].size(); i++ )
    {
        if(sessions_map[username][i] == session_id)
        {
            cout << "Already logged in" << NEW_LINE;
            return_code = -1;
        }
    }
    if(return_code != -1)
    {
        sessions_map[username].push_back(session_id);
    }

    return return_code;
}