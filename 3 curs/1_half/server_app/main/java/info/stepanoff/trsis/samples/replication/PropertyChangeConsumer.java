package info.stepanoff.trsis.samples.replication;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import info.stepanoff.trsis.samples.db.dao.*;
import info.stepanoff.trsis.samples.db.model.*;
import info.stepanoff.trsis.samples.rest.model.PropertyDTO;
import lombok.RequiredArgsConstructor;
import org.springframework.boot.autoconfigure.condition.ConditionalOnProperty;
import org.springframework.kafka.annotation.KafkaListener;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

@Service
@RequiredArgsConstructor
@ConditionalOnProperty(name = "app.replication.enabled", havingValue = "true")
public class PropertyChangeConsumer {
    private final ObjectMapper mapper;
    private final PropertyRepository properties;
    private final UserRepository users;
    private final AuditEventRepository audit;
    private final AppliedEventRepository applied;

    @Transactional
    @KafkaListener(topics = "${app.replication.topic}", groupId = "#{@replicationGroup}")
    public void receive(String json) throws JsonProcessingException {
        PropertyChange event = mapper.readValue(json, PropertyChange.class);
        if (applied.existsById(event.eventId())) return;
        PropertyDTO source = event.property();
        if ("DELETE".equals(event.action())) {
            properties.findById(source.getId()).ifPresent(properties::delete);
        } else if ("CREATE".equals(event.action()) || "UPDATE".equals(event.action())) {
            PropertyPE entity = properties.findById(source.getId()).orElseGet(PropertyPE::new);
            entity.setId(source.getId());
            entity.setAddress(source.getAddress());
            entity.setType(source.getType());
            entity.setArea(source.getArea());
            entity.setRooms(source.getRooms());
            entity.setMonthlyRent(source.getMonthlyRent());
            entity.setAvailable(source.getAvailable());
            entity.setOwner(users.findByLogin(source.getOwnerLogin()).orElseThrow());
            properties.save(entity);
        } else {
            throw new IllegalArgumentException("Unknown action " + event.action());
        }
        AuditEventPE entry = new AuditEventPE();
        entry.setActor(event.actor());
        entry.setAction(event.action());
        entry.setPropertyId(source.getId());
        entry.setOccurredAt(event.occurredAt());
        audit.save(entry);
        applied.save(new AppliedEvent(event.eventId()));
        // Do not call PropertyService: applying an event must not produce another one.
    }
}
