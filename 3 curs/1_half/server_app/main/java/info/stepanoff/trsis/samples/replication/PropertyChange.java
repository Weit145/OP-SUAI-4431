package info.stepanoff.trsis.samples.replication;

import info.stepanoff.trsis.samples.rest.model.PropertyDTO;
import java.time.Instant;

public record PropertyChange(String eventId, String source, String action,
                             String actor, Instant occurredAt, PropertyDTO property) {
}
