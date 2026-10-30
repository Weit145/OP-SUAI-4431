package info.stepanoff.trsis.samples.replication;

import jakarta.persistence.*;
import java.time.Instant;
import lombok.Getter;
import lombok.Setter;

@Entity
@Table(name = "REPLICATION_OUTBOX")
@Getter
@Setter
public class OutboxEvent {
    @Id
    @Column(name = "EVENT_ID")
    private String id;
    @Column(name = "PROPERTY_ID", nullable = false)
    private Long propertyId;
    @Column(name = "PAYLOAD", nullable = false, length = 10000)
    private String payload;
    @Column(name = "CREATED_AT", nullable = false)
    private Instant createdAt;
}
