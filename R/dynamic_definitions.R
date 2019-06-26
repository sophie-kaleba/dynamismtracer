dynamic_definitions_analysis <- create_analysis(

    id = "dynamic_definitions",

    reducer = function(analyses) {

        if(nrow(analyses$dynamic_function_definitions) == 0) {
            return(list())
        }

        list(dynamic_function_definitions = analyses$dynamic_function_definitions)
    },

    summarizer = function(analyses) {

        encode_sequence <- function(seq) {
            str_c("(", str_c(seq, collapse = " "), ")", sep = "")
        }

        decode_sequence <- function(seq) {
            unlist(str_split(str_sub(seq, 2, -2), " "))
        }

        dynamic_function_definitions <-
            analyses$dynamic_function_definitions %>%
            group_by(function_id) %>%
            summarize(formal_parameter_count = first(formal_parameter_count),
                      package = first(package),
                      function_name = encode_sequence(unique(decode_sequence(function_name))),
                      definition = first(definition),
                      script = encode_sequence(unique(decode_sequence(str_c(package, script_type, script_name,
                                                     sep = "/"))))) %>%
            ungroup()

        list(dynamic_function_definitions = dynamic_function_definitions)
    }
)
