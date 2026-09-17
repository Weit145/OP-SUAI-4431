package info.stepanoff.trsis.samples.rest;

import io.swagger.v3.oas.annotations.media.Schema;
import java.time.Instant;
import java.util.Map;

@Schema(description = "Ошибка API")
public record ApiError(Instant timestamp, int status, String error, String message,
                       Map<String, String> validationErrors) {
}
