#!/usr/bin/env python

"""
python functions to communicate with the Sheep server (C++ REST API)
All functions should return a json structure
{"status_code": <int>, "content": x }
where x can be a string (if there is an error) or a json structure.
In all cases, should catch ConnectionError in case the server is down,
so that this can be communicated to the frontend.
"""

import os
import requests
import json

from . import database
from . import common_utils

if "SERVER_URL_BASE" in os.environ.keys():
    BASE_URI = os.environ["SERVER_URL_BASE"]
else:
    BASE_URI = "http://localhost:34568/SheepServer"

def is_configured():
    """
    check if circuit, inputs, input_type, and context have all been set.
    """
    response_dict = {}
    try:
        r = requests.get(BASE_URI+"/job/")
        response_dict["status_code"] = r.status_code
        response_dict["content"] = json.loads(r.content.decode("utf-8"))
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to check configuration"
    return response_dict


def get_available_contexts():
    """
    get a list of the available contexts.
    """
    response_dict = {}
    try:
        r = requests.get(BASE_URI+"/context/")
        response_dict["status_code"] = r.status_code
        result = json.loads(r.content.decode("utf-8"))
        if r.status_code == 200:
            response_dict["content"] = result["contexts"]
        else:
            response_dict["content"] = result
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to get context list"
    return response_dict


def get_available_input_types():
    """
    get a list of the available input types.
    If the context is already
    """
    response_dict = {}
    try:
        r = requests.get(BASE_URI+"/input_type/")
        response_dict["status_code"] = r.status_code
        result = json.loads(r.content.decode("utf-8"))
        input_types = result["input_types"]
        ## see if we have a CKKS context set
        if get_context()["content"]:
            if "CKKS" in get_context()["content"]:
                input_types = ["double","complex"]
            else:
                input_types.remove("double")
                input_types.remove("complex")
        if r.status_code == 200:
            response_dict["content"] = input_types
        else:
            response_dict["content"] = result
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to get list of input types"
    return response_dict


def set_context(context_name):
    """
    set a context.  First check it is in the list of available ones.
    """
    if not isinstance(context_name, str):
        return {"status_code": 500, "content": "incorrect input type for set_context"}
    context_request = get_available_contexts()
    if context_request["status_code"] != 200:
        return context_request
    response_dict = {}
    available_contexts = context_request["content"]
    if not context_name in available_contexts:
        response_dict["status_code"] = 500
        response_dict["content"] = "context {} is not in {}".format(context_name,
                                                                    available_contexts)
        return response_dict
    try:
        r = requests.post(BASE_URI+"/context/",
                          json={"context_name": context_name})
        response_dict["status_code"] = r.status_code
        response_dict["content"] = r.content.decode("utf-8")
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to set context"
    return response_dict


def set_input_type(input_type):
    """
    set input type, if it is in the list of available types.
    """
    if not isinstance(input_type, str):
        return {"status_code": 500, "content": "incorrect input type for set_input_type"}
    type_request = get_available_input_types()
    if type_request["status_code"] != 200:
        return type_request
    response_dict = {}
    available_types = type_request["content"]
    if not input_type in available_types:
        response_dict["status_code"] = 500
        response_dict["content"] = "input_type {} not in {}".format(input_type,
                                                                    available_types)
        return response_dict
    try:
        r = requests.post(BASE_URI+"/input_type/",
                          json={"input_type": input_type})
        response_dict["status_code"] = r.status_code
        response_dict["content"] = r.content.decode("utf-8")
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to set input type"
    return response_dict


def get_inputs():
    """
    parse the circuit and get the list of inputs.
    """
    response_dict = {}
    try:
        r = requests.get(BASE_URI+"/inputs/")
        response_dict["status_code"] = r.status_code
        result = json.loads(r.content.decode("utf-8"))
        if r.status_code == 200:
            response_dict["content"] = result["inputs"]
        else:
            response_dict["content"] = "Error getting inputs - do we have a circuit?"
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to get inputs"
    return response_dict


def get_const_inputs():
    """
    parse the circuit and get the list of inputs.
    """
    response_dict = {}
    try:
        r = requests.get(BASE_URI+"/const_inputs/")
        response_dict["status_code"] = r.status_code
        result = json.loads(r.content.decode("utf-8"))
        if r.status_code == 200:
            response_dict["content"] = result["const_inputs"]
        else:
            response_dict["content"] = "Error getting inputs - do we have a circuit?"
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to get inputs"
    return response_dict


def set_inputs(input_dict):
    """
    set input values.
    """
    if not isinstance(input_dict, dict):
        return {"status_code": 500, "content": "incorrect input type for set_inputs"}
    input_request = get_inputs()
    if input_request["status_code"] != 200:
        return input_request
    input_names = input_request["content"]
    response_dict = {}

    unset_inputs = [i for i in input_names if not i in input_dict.keys()]

    if len(unset_inputs) > 0:
        response_dict["status_code"] = 500
        response_dict["content"] = "Inputs {} not set".format(unset_inputs)
        return response_dict
    unused_inputs = [i for i in input_dict.keys() if not i in input_names]
    if len(unused_inputs) > 0:
        response_dict["status_code"] = 500
        response_dict["content"] = "Inputs {} are not inputs to the circuit".format(unused_inputs)
        return response_dict
    try:
        input_type = get_config()["content"]["input_type"]
    except(KeyError):
        response_dict["status_code"] = 500
        response_dict["content"] = "Input type not set"
        return response_dict
    ## check we don't have too many input values for the number of slots
    num_slots_request = get_nslots()
    if num_slots_request["status_code"] != 200:
        return num_slots_request
    num_slots = num_slots_request["content"]["nslots"]
    input_dict = common_utils.convert_inputs_to_str(input_dict, input_type)
    consistent_inputs = common_utils.check_inputs(input_dict, input_type, num_slots)
    if not consistent_inputs:
        response_dict["status_code"] = 500
        response_dict["content"] = "Inputs are wrong type, or different lengths, or > nslots values per wire"
        return response_dict
    #
    try:
        r = requests.post(BASE_URI+"/inputs/",
                          json=input_dict)
        response_dict["status_code"] = r.status_code
        response_dict["content"] = r.content.decode("utf-8")
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to set inputs"
    return response_dict


def set_const_inputs(input_dict):
    """
    set input values.
    """
    if not isinstance(input_dict, dict):
        return {"status_code": 500, "content": "incorrect input type for set_inputs"}
    input_request = get_const_inputs()
    if input_request["status_code"] != 200:
        return input_request
    input_names = input_request["content"]
    response_dict = {}
    unset_inputs = [i for i in input_names if not i in input_dict.keys()]
    if len(unset_inputs) > 0:
        response_dict["status_code"] = 500
        response_dict["content"] = "Inputs {} not set".format(unset_inputs)
        return response_dict
    unused_inputs = [i for i in input_dict.keys() if not i in input_names]
    if len(unused_inputs) > 0:
        response_dict["status_code"] = 500
        response_dict["content"] = "Inputs {} are not inputs to the circuit".format(unused_inputs)
        return response_dict
    try:
        r = requests.post(BASE_URI+"/const_inputs/",
                          json=input_dict)
        response_dict["status_code"] = r.status_code
        response_dict["content"] = r.content.decode("utf-8")
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to set inputs"
    return response_dict


def set_circuit_filename(circuit_filename):
    """
    Specify full path to circuit filename.
    """
    if not isinstance(circuit_filename, str):
        return {"status_code": 500, "content": "incorrect input type for set_circuit_filename"}
    response_dict = {}
    try:
        r = requests.post(BASE_URI+"/circuitfile/",
                          json={"circuit_filename": circuit_filename})
        response_dict["status_code"] = r.status_code
        response_dict["content"] = r.content
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to set circuit filename"
    return response_dict


def set_circuit(circuit_filename):
    """
    Read the circuit and pass it to the server as a string.
    """
    if not isinstance(circuit_filename, str):
        return {"status_code": 500, "content": "incorrect input type for set_circuit"}
    response_dict = {}
    if not os.path.exists(circuit_filename):
        response_dict["status_code"] = 500
        response_dict["content"] = "Circuit file not found"
        return response_dict
    try:
        r = requests.post(BASE_URI+"/circuit/",
                          json={"circuit": open(circuit_filename).read()})
        response_dict["status_code"] = r.status_code
        response_dict["content"] = r.content.decode("utf-8")
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to set circuit"
    return response_dict


def set_circuit_text(circuit_text):
    """
    Read the circuit and pass it to the server as a string.
    """
    if not isinstance(circuit_text, str):
        return {"status_code": 500, "content": "incorrect input type for set_circuit"}
    response_dict = {}
    try:
        r = requests.post(BASE_URI+"/circuit/",
                          json={"circuit": circuit_text})
        response_dict["status_code"] = r.status_code
        response_dict["content"] = r.content.decode("utf-8")
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to set circuit"
    return response_dict


def get_parameters():
    """
    Will instantiate a context and query it for its parameters"
    """
    response_dict = {}
    try:
        r = requests.get(BASE_URI+"/parameters/")
        response_dict["status_code"] =  r.status_code
        response_dict["content"] = json.loads(r.content.decode("utf-8"))
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to get parameters"
    return response_dict


def get_nslots():
    """
    Will instantiate a context with the current parameter set,
    and query it for how many slots it can handle.
    """
    response_dict = {}
    try:
        r = requests.get(BASE_URI+"/slots/")
        response_dict["status_code"] = r.status_code
        response_dict["content"] = json.loads(r.content.decode("utf-8"))
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to get parameters"
    return response_dict


def set_parameters(param_dict):
    """
    Set the parameters from a dict { param_name: param_val}
    """
    if not isinstance(param_dict, dict):
        return {"status_code": 500, "content": "incorrect input type for set_parameters"}
    response_dict = {}
    try:
        r=requests.put(BASE_URI+"/parameters/",
                       json=param_dict)
        response_dict["status_code"] = r.status_code
        response_dict["content"] = r.content.decode("utf-8")
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to set parameters"
    return response_dict


def encrypt_and_serialize(plaintext_vec):
    """
    Given a vector of plaintexts, encrypt them and get the string
    of the serialized output
    """
    nslot_request = get_nslots()
    if nslot_request["status_code"] != 200:
        return nslot_request
    nslots = nslot_request["content"]["nslots"]

    if len(plaintext_vec) > nslots:
        return {"status_code": 500, "content": "Not enough slots"}
    response_dict = {}
    try:
        r=requests.post(BASE_URI+"/serialized_ct/", json={"inputs":plaintext_vec})
        if r.status_code != 200:
            return r
        response_dict["status_code"] = 200
        response_dict["content"] = json.loads(r.content.decode("utf-8"))
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to set parameters"
    return response_dict


def set_eval_strategy(strategy):
    """
    choose between serial and parallel evaluation.
    """
    if not isinstance(strategy, str):
        return {"status_code": 500, "content": "incorrect input type for set_strategy"}
    response_dict = {}
    if not strategy in ["serial","parallel"]:
        response_dict["status_code"] = 500
        response_dict["content"] = "Eval strategy must be 'serial' or 'parallel'"
        return response_dict
    try:
        r=requests.put(BASE_URI+"/eval_strategy/",
                       json={"eval_strategy": strategy})
        response_dict["status_code"] = r.status_code
        response_dict["content"] = r.content.decode("utf-8")
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to set eval strategy"
    return response_dict


def get_eval_strategy():
    """
    see whether the job is set to run in serial or parallel
    """
    response_dict = {}
    try:
        r=requests.get(BASE_URI+"/eval_strategy/")
        response_dict["status_code"] = r.status_code
        response_dict["content"] = json.loads(r.content.decode("utf-8"))
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to get eval strategy"
    return response_dict


def new_job():
    """
    reset all the job configuration and results structs on the server.
    """
    response_dict = {}
    try:
        r=requests.post(BASE_URI+"/job/")
        response_dict["status_code"] = r.status_code
        response_dict["content"] = r.content.decode("utf-8")
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server for new job"
    return response_dict


def run_job():
    """
    execute the job, if it is fully configured.
    """
    response_dict = {}
    config_request = is_configured()
    if config_request["status_code"] != 200:
        return config_request
    if not config_request["content"]["job_configured"]:
        response_dict["status_code"] = 500
        response_dict["content"] = "Job not fully configured"
        return response_dict
    try:
        r = requests.post(BASE_URI+"/run/")
        response_dict["status_code"] = r.status_code
        response_dict["content"] = r.content.decode("utf-8")
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to run job"
    return response_dict


def get_config():
    """
    get a json object with all the configuration - inputs, parameters etc.
    """
    response_dict = {}
    try:
        r = requests.get(BASE_URI+"/config/")
        response_dict["status_code"] = r.status_code
        response_dict["content"] = json.loads(r.content.decode("utf-8"))
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to get config"
    return response_dict


def get_context():
    """
    Get the currently set context
    """
    response_dict = {}
    try:
        r = requests.get(BASE_URI+"/config/")
        response_dict["status_code"] = r.status_code
        response_dict["content"] = json.loads(r.content.decode("utf-8"))['context']
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to get config"
    return response_dict


def get_results():
    """
    retrieve results - should be a dictionary containing
    a dictionary of outputs, and a dictionary of timings.
    """
    response_dict = {}
    try:
        r=requests.get(BASE_URI+"/results/")
        response_dict["status_code"] = r.status_code
        response_dict["content"] = json.loads(r.content.decode("utf-8"))
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to get results"
    return response_dict




def get_circuit():
    """
    Get the server to return its circuit as a string
    """
    response_dict = {}
    try:
        r=requests.get(BASE_URI+"/circuit/")
        response_dict["status_code"] = r.status_code
        response_dict["content"] = json.loads(r.content.decode("utf-8"))
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to get results"
    return response_dict



def set_timeout(timeout):
    """
    set the timeout (in seconds)
    """
    if not isinstance(timeout, int):
        return {"status_code": 500, "content": "incorrect input type for set_timeout"}
    response_dict = {}

    try:
        r=requests.put(BASE_URI+"/timeout/",
                       json={"timeout": timeout})
        response_dict["status_code"] = r.status_code
        response_dict["content"] = r.content.decode("utf-8")
    except(requests.exceptions.ConnectionError):
        response_dict["status_code"] = 404
        response_dict["content"] = "Unable to connect to SHEEP server to set timeout"
    return response_dict
