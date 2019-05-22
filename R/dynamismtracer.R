
create_dyntracer <- function(output_dirpath,
                             verbose = FALSE,
                             truncate = TRUE,
                             binary = FALSE,
                             compression_level = 0) {

  compression_level <- as.integer(compression_level)

  .Call(C_create_dyntracer,
        output_dirpath,
        verbose,
        truncate,
        binary,
        compression_level)
}


destroy_dyntracer <- function(dyntracer) {
  invisible(.Call(C_destroy_dyntracer, dyntracer))
}

# trigger the profiling of the expression given as input
dyntrace_dynamism <- function(expr,
                              output_dirpath,
                              verbose = FALSE,
                              truncate = TRUE,
                              binary = FALSE,
                              compression_level = 0) {

  write(as.character(Sys.time()), file.path(output_dirpath, "BEGIN"))

  compression_level <- as.integer(compression_level)

  dyntracer <- create_dyntracer(output_dirpath,
                                verbose,
                                truncate,
                                binary,
                                compression_level)

  result <- dyntrace(dyntracer, expr)

  destroy_dyntracer(dyntracer)

  write(as.character(Sys.time()), file.path(output_dirpath, "FINISH"))

  result
}
