package info.stepanoff.trsis.samples.db.dao;

import info.stepanoff.trsis.samples.db.model.AuditEventPE;
import java.util.List;
import org.springframework.data.jpa.repository.JpaRepository;

public interface AuditEventRepository extends JpaRepository<AuditEventPE, Long> {
    List<AuditEventPE> findAllByOrderByIdDesc();
}
