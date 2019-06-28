dynamic_calls_analysis <- create_analysis(

    id = "dynamic_calls",

    reducer = function(analyses) {

        if(nrow(analyses$dynamic_call_summaries) == 0) {
            return(list())
        }

        list(dynamic_call_summaries = analyses$dynamic_call_summaries)
    },

    summarizer = function(analyses) {

        encode_sequence <- function(seq) {
            str_c("(", str_c(seq, collapse = " "), ")", sep = "")
        }

        decode_sequence <- function(seq) {
            unlist(str_split(str_sub(seq, 2, -2), " "))
        }

        dynamic_call_summaries <-
            analyses$dynamic_call_summaries %>%
            group_by(function_id, parent_id) %>%
            summarize(formal_parameter_count = first(formal_parameter_count),
		      dyn_call_count = sum(dyn_call_count),
		      call_count = sum(call_count),
                      package = first(package),
		      redifining = sum(redefining_symbol), 
                      function_name = encode_sequence(unique(decode_sequence(function_name))),
                      script = encode_sequence(str_c(package, script_type, script_name,
                                                     sep = "/"))) %>%
            ungroup()

        list(dynamic_call_summaries = dynamic_call_summaries)
    }
)
