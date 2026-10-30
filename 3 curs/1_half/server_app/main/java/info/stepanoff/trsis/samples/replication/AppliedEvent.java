package info.stepanoff.trsis.samples.replication;

import jakarta.persistence.*;
import lombok.NoArgsConstructor;

@Entity
@Table(name = "REPLICATION_APPLIED")
@NoArgsConstructor
public class AppliedEvent {
    @Id
    @Column(name = "EVENT_ID")
    private String id;

    public AppliedEvent(String id) {
        this.id = id;
    }
}
