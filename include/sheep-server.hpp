#ifndef SHEEP_SERVER_HPP
#define SHEEP_SERVER_HPP

#include <memory>
#include <fstream>
#include <map>

#include "context-clear.hpp"
#include "context-helib.hpp"
#include "context-tfhe.hpp"
//#include "context-seal.hpp"


using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

static std::vector<std::string> available_contexts = {"HElib_Fp","HElib_F2","TFHE"};


struct SheepJobConfig {
  utility::string_t context;
  utility::string_t circuit_filename;
  utility::string_t input_filename;

  void reset() {
    context = "";
    circuit_filename = "";
    input_filename = "";
  }
  bool isConfigured() {
    return ((context.size() > 0) && (circuit_filename.size() > 0) && (input_filename.size() > 0) );
  }
};

struct People
{
	utility::string_t name;
	double age;

	static People FromJSON(const web::json::object & object)
	{
		People result;
		result.name = object.at(U("name")).as_string();
		result.age = object.at(U("age")).as_integer();
		return result;
	}

	web::json::value AsJSON() const
	{
		web::json::value result = web::json::value::object();
		result[U("name")] = web::json::value::string(name);
		result[U("age")] = web::json::value::number(age);
		return result;
	}
};

struct Data
{
	std::vector<People> peoples;
	utility::string_t job;

	Data() {}

	void Clear() { peoples.clear(); }

	static Data FromJSON(const web::json::object &object)
	{
		Data res;

		web::json::value cs = object.at(U("data"));

		for (auto iter = cs.as_array().begin(); iter != cs.as_array().end(); ++iter)
		{
			if (!iter->is_null())
			{
				People people;
				people = People::FromJSON(iter->as_object());
				res.peoples.push_back(people);
			}
		}

		auto job = object.find(U("job"));
		res.job = job->second.as_string();
		return res;
	}

	web::json::value AsJSON() const
	{
		web::json::value res = web::json::value::object();
		res[U("job")] = web::json::value::string(job);

		web::json::value jPeoples = web::json::value::array(peoples.size());

		int idx = 0;
		for (auto iter = peoples.begin(); iter != peoples.end(); iter++)
		{
			jPeoples[idx++] = iter->AsJSON();
		}

		res[U("people")] = jPeoples;
		return res;
	}
};

class SheepServer
{
public:
	SheepServer() {}
	SheepServer(utility::string_t url);

	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }

  template <typename PlaintextT>
  std::unique_ptr<BaseContext<PlaintextT> >
  make_context(std::string context_type);

private:

        void handle_get_context(http_request message);
        void handle_post_context(http_request message);
        void handle_get_job(http_request message);
        void handle_post_job(http_request message);
        void handle_get_parameters(http_request message);
        void handle_put_parameters(http_request message);  
	void handle_get(http_request message);
	void handle_put(http_request message);
	void handle_post(http_request message);

	http_listener m_listener;

        SheepJobConfig m_job_config;
  //        typename T;
        BaseBaseContext* m_context;

};

#endif