#pragma once

typedef struct {
	size_t end;
} tokens_iter_result_t;

typedef tokens_iter_result_t(*tokens_iter_t)(tokens_slice_t*);
