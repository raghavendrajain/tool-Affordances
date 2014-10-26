#include "StopMotion.h"

template <typename T> string tostr(const T& t)
{ 
  ostringstream os; 
  os<<t; 
  return os.str(); 
} // template to convert double variables to string




double* stopMotion(SimObj *target, Vector3d desiredTargetVelocity, double K_p, double K_i, double K_d )
{
	Vector3d currentTargetVelocity;

	target->getLinearVelocity(currentTargetVelocity);
	cout << "The currentTargetVelocity = " << currentTargetVelocity.x() << " , " << currentTargetVelocity.z() << endl; 

    double *pointer;
    double P_value[1];
    pointer=P_value;
    double P_Value_X;
    double P_Value_Z;

 
    dlopen("libpython2.7.so", RTLD_LAZY | RTLD_GLOBAL); 
    Py_Initialize();  //initialization of the python interpreter

  try
    {
      // load the main module
       py::object main_module = py::import("__main__");
      // // load the dictionary object out of the main module to create a blank canvas on which python variables and functions can be executed. 
       py::object main_namespace = main_module.attr("__dict__");

       main_module.attr("K_p") = K_p; 
       main_module.attr("K_i") = K_i; 
       main_module.attr("K_d") = K_d; 


       main_module.attr("desiredTargetVelocity") = "[" + tostr(desiredTargetVelocity.x())+" , "+ tostr(desiredTargetVelocity.y())+ " , " + tostr(desiredTargetVelocity.z()) + "]";
       main_module.attr("currentTargetVelocity") = "[" + tostr(currentTargetVelocity.x())+" , "+ tostr(currentTargetVelocity.y())+ " , " + tostr(currentTargetVelocity.z()) + "]";
      
       py::exec("import ast", main_namespace);
       py::exec("desiredTargetVelocity = ast.literal_eval(desiredTargetVelocity)", main_namespace);
       // py::exec("print goalPos", main_namespace);
       py::exec("currentTargetVelocity = ast.literal_eval(currentTargetVelocity)", main_namespace);
       // py::exec("print currentPos", main_namespace);
       py::exec("import PID as pid", main_namespace);
       py::exec("p_x=pid.PID(K_p, K_i, K_d)",main_namespace);
       py::exec("p_z=pid.PID(K_p, K_i, K_d)",main_namespace);
       py::exec("p_x.setPoint(desiredTargetVelocity[0])", main_namespace);
       py::exec("p_z.setPoint(desiredTargetVelocity[2])", main_namespace);
       // py::exec("print p_z.getPoint()", main_namespace);
       py::exec("pid_x=p_x.update(currentPos[0])", main_namespace);
       py::exec("pid_z=p_z.update(currentPos[2])", main_namespace);
       
       // cout << "Update in x is " << py::extract<double>(main_module.attr("pid_x")) << endl;
       // cout << "Update in z is " << py::extract<double>(main_module.attr("pid_z")) << endl;
       P_Value_X = py::extract<double>(main_module.attr("pid_x"));
       P_Value_Z = py::extract<double>(main_module.attr("pid_z"));

       P_value[0] = P_Value_X;
       P_value[1] = P_Value_Z;

       return pointer;
    
    }



     catch(boost::python::error_already_set const &){
        // Parse and output the exception
        std::string perror_str = parse_python_exception();
        std::cout << "Error in Python: " << perror_str << std::endl;
    }

}



std::string parse_python_exception(){
    PyObject *type_ptr = NULL, *value_ptr = NULL, *traceback_ptr = NULL;
    // Fetch the exception info from the Python C API
    PyErr_Fetch(&type_ptr, &value_ptr, &traceback_ptr);

    // Fallback error
    std::string ret("Unfetchable Python error");
    // If the fetch got a type pointer, parse the type into the exception string
    if(type_ptr != NULL){
        py::handle<> h_type(type_ptr);
        py::str type_pstr(h_type);
        // Extract the string from the boost::python object
        py::extract<std::string> e_type_pstr(type_pstr);
        // If a valid string extraction is available, use it 
        //  otherwise use fallback
        if(e_type_pstr.check())
            ret = e_type_pstr();
        else
            ret = "Unknown exception type";
    }
    // Do the same for the exception value (the stringification of the exception)
    if(value_ptr != NULL){
        py::handle<> h_val(value_ptr);
        py::str a(h_val);
        py::extract<std::string> returned(a);
        if(returned.check())
            ret +=  ": " + returned();
        else
            ret += std::string(": Unparseable Python error: ");
    }
    // Parse lines from the traceback using the Python traceback module
    if(traceback_ptr != NULL){
        py::handle<> h_tb(traceback_ptr);
        // Load the traceback module and the format_tb function
        py::object tb(py::import("traceback"));
        py::object fmt_tb(tb.attr("format_tb"));
        // Call format_tb to get a list of traceback strings
        py::object tb_list(fmt_tb(h_tb));
        // Join the traceback strings into a single string
        py::object tb_str(py::str("\n").join(tb_list));
        // Extract the string, check the extraction, and fallback in necessary
        py::extract<std::string> returned(tb_str);
        if(returned.check())
            ret += ": " + returned();
        else
            ret += std::string(": Unparseable Python traceback");
    }

}