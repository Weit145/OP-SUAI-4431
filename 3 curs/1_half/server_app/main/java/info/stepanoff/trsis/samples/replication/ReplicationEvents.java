package info.stepanoff.trsis.samples.replication;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import info.stepanoff.trsis.samples.rest.model.PropertyDTO;
import java.time.Instant;
import java.util.UUID;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

@Service
@RequiredArgsConstructor
public class ReplicationEvents {
    private final OutboxRepository outbox;
    private final ObjectMapper mapper;
    @Value("${app.replication.enabled:false}")
    private boolean enabled;
    @Value("${app.instance-id}")
    private String instance;

    // The property and its outgoing event commit or roll back together.
    @Transactional(propagation = Propagation.MANDATORY)
    public boolean record(String action, PropertyDTO property, String actor) {
        if (!enabled) return false;
        PropertyChange change = new PropertyChange(UUID.randomUUID().toString(), instance,
                action, actor, Instant.now(), property);
        OutboxEvent event = new OutboxEvent();
        event.setId(change.eventId());
        event.setPropertyId(property.getId());
        event.setCreatedAt(change.occurredAt());
        try {
            event.setPayload(mapper.writeValueAsString(change));
        } catch (JsonProcessingException exception) {
            throw new IllegalStateException("Cannot serialize replication event", exception);
        }
        outbox.save(event);
        return true;
    }
}
